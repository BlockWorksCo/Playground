#!/usr/bin/env python3

import dbus
try:
  from gi.repository import GObject
except ImportError:
  import gobject as GObject
import sys

from dbus.mainloop.glib import DBusGMainLoop

bus = None
mainloop = None

BLUEZ_SERVICE_NAME = 'org.bluez'
DBUS_OM_IFACE =      'org.freedesktop.DBus.ObjectManager'
DBUS_PROP_IFACE =    'org.freedesktop.DBus.Properties'

GATT_SERVICE_IFACE = 'org.bluez.GattService1'
GATT_CHRC_IFACE =    'org.bluez.GattCharacteristic1'

UART_TX_UUID        = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'
UART_RX_UUID        = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
UART_UUID           = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'

HR_SVC_UUID =        '0000180d-0000-1000-8000-00805f9b34fb'
HR_MSRMT_UUID =      '00002a37-0000-1000-8000-00805f9b34fb'
BODY_SNSR_LOC_UUID = '00002a38-0000-1000-8000-00805f9b34fb'
HR_CTRL_PT_UUID =    '00002a39-0000-1000-8000-00805f9b34fb'

# The objects that we interact with.
hr_service = None
hr_msrmt_chrc = None
body_snsr_loc_chrc = None
hr_ctrl_pt_chrc = None


def generic_error_cb(error):
    print('D-Bus call failed: ' + str(error))
    mainloop.quit()


def body_sensor_val_to_str(val):
    if val == 0:
        return 'Other'
    if val == 1:
        return 'Chest'
    if val == 2:
        return 'Wrist'
    if val == 3:
        return 'Finger'
    if val == 4:
        return 'Hand'
    if val == 5:
        return 'Ear Lobe'
    if val == 6:
        return 'Foot'

    return 'Reserved value'


def sensor_contact_val_to_str(val):
    if val == 0 or val == 1:
        return 'not supported'
    if val == 2:
        return 'no contact detected'
    if val == 3:
        return 'contact detected'

    return 'invalid value'


def body_sensor_val_cb(value):
    if len(value) != 1:
        print('Invalid body sensor location value: ' + repr(value))
        return

    print('Body sensor location value: ' + body_sensor_val_to_str(value[0]))


def hr_msrmt_start_notify_cb():
    print('HR Measurement notifications enabled')


def hr_msrmt_changed_cb(iface, changed_props, invalidated_props):
    if iface != GATT_CHRC_IFACE:
        return

    if not len(changed_props):
        return

    value = changed_props.get('Value', None)
    if not value:
        return

    print('New HR Measurement')

    flags = value[0]
    value_format = flags & 0x01
    sc_status = (flags >> 1) & 0x03
    ee_status = flags & 0x08

    if value_format == 0x00:
        hr_msrmt = value[1]
        next_ind = 2
    else:
        hr_msrmt = value[1] | (value[2] << 8)
        next_ind = 3

    print('\tHR: ' + str(int(hr_msrmt)))
    print('\tSensor Contact status: ' +
          sensor_contact_val_to_str(sc_status))

    if ee_status:
        print('\tEnergy Expended: ' + str(int(value[next_ind])))


def start_client():
    # Read the Body Sensor Location value and print it asynchronously.
    body_snsr_loc_chrc[0].ReadValue({}, reply_handler=body_sensor_val_cb,
                                    error_handler=generic_error_cb,
                                    dbus_interface=GATT_CHRC_IFACE)

    # Listen to PropertiesChanged signals from the Heart Measurement
    # Characteristic.
    hr_msrmt_prop_iface = dbus.Interface(hr_msrmt_chrc[0], DBUS_PROP_IFACE)
    hr_msrmt_prop_iface.connect_to_signal("PropertiesChanged",
                                          hr_msrmt_changed_cb)

    # Subscribe to Heart Rate Measurement notifications.
    hr_msrmt_chrc[0].StartNotify(reply_handler=hr_msrmt_start_notify_cb,
                                 error_handler=generic_error_cb,
                                 dbus_interface=GATT_CHRC_IFACE)


def process_chrc(chrc_path):
    chrc = bus.get_object(BLUEZ_SERVICE_NAME, chrc_path)
    chrc_props = chrc.GetAll(GATT_CHRC_IFACE,
                             dbus_interface=DBUS_PROP_IFACE)

    uuid = chrc_props['UUID']

    if uuid == HR_MSRMT_UUID:
        global hr_msrmt_chrc
        hr_msrmt_chrc = (chrc, chrc_props)
    elif uuid == BODY_SNSR_LOC_UUID:
        global body_snsr_loc_chrc
        body_snsr_loc_chrc = (chrc, chrc_props)
    elif uuid == HR_CTRL_PT_UUID:
        global hr_ctrl_pt_chrc
        hr_ctrl_pt_chrc = (chrc, chrc_props)
    else:
        print('Unrecognized characteristic: ' + uuid)

    return True


def process_hr_service(service_path, chrc_paths):
    service = bus.get_object(BLUEZ_SERVICE_NAME, service_path)
    service_props = service.GetAll(GATT_SERVICE_IFACE,
                                   dbus_interface=DBUS_PROP_IFACE)

    uuid = service_props['UUID']

    if uuid != UART_UUID:
        return False

    print('UART Service found: ' + service_path)

    # Process the characteristics.
    for chrc_path in chrc_paths:
        process_chrc(chrc_path)

    global hr_service
    hr_service = (service, service_props, service_path)

    return True


def interfaces_removed_cb(object_path, interfaces):
    if not hr_service:
        return

    if object_path == hr_service[2]:
        print('Service was removed')
        mainloop.quit()


def cb(value):
    print('cb [%s]'%(str(value)))

def main():
    # Set up the main loop.
    DBusGMainLoop(set_as_default=True)
    global bus
    bus = dbus.SystemBus()
    global mainloop
    mainloop = GObject.MainLoop()

    om = dbus.Interface(bus.get_object(BLUEZ_SERVICE_NAME, '/'), DBUS_OM_IFACE)
    om.connect_to_signal('InterfacesRemoved', interfaces_removed_cb)

    print('Getting objects...')
    objects = om.GetManagedObjects()
    #chrcs = []

    # List characteristics found
    #for path, interfaces in objects.items():
        #print(path)
        #print(interfaces)
        #if GATT_CHRC_IFACE not in interfaces.keys():
            #continue
        #chrcs.append(str(path))

    #print()
    #for c in sorted(chrcs):
        #print(c)
    #print()

    #print('Objects:')
    #for o in objects.keys():
        #try:
            #uuid    = objects[o]['UUID']
        #except KeyError:
            #uuid    = ''
        #print(o + ' ' + uuid)
    #print()

    #
    # Find all object paths that have a matching UUID and make a list of them.
    #
    objectPathsForUUID  = []
    for path, interfaces in objects.items():

        if GATT_SERVICE_IFACE not in interfaces.keys():
            continue

        if UART_UUID in interfaces['org.bluez.GattService1']['UUID'].upper():
            objectPathsForUUID.append(str(path))


    print()
    print(objectPathsForUUID)
    print()

    #
    # Find all characteristics that belong to the list of objects from above.
    #
    characteristicPathsForUUID  = []
    characteristicInterfacesForUUID  = {}
    for path, interfaces in objects.items():

        for p in objectPathsForUUID:
            if path.startswith(p+'/'):
                characteristicPathsForUUID.append(str(path))
                characteristicInterfacesForUUID[str(path)]  = interfaces


    for c in characteristicPathsForUUID:
        if 'org.bluez.GattCharacteristic1' in characteristicInterfacesForUUID[c]:
            print()
            print(c)
            d   = characteristicInterfacesForUUID[c]['org.bluez.GattCharacteristic1']
            print(d)
            for k in d:
                print('\t%s = %s'%(k,d[k]))


    # write to  /org/bluez/hci0/dev_B8_27_EB_12_E5_84/service0049/char004d
    # read from /org/bluez/hci0/dev_B8_27_EB_12_E5_84/service0049/char004a

    #chrObj=bus.get_object('org.bluez','/org/bluez/hci0/dev_B8_27_EB_12_E5_84/service0049/char004d')
    chrObj=bus.get_object('org.bluez','/org/bluez/hci0/dev_B8_27_EB_12_E5_84/service0057/char005b')
    propIf=dbus.Interface(chrObj, 'org.bluez.GattCharacteristic1')
    print('trying to write...')
    #propIf.ReadValue({}, reply_handler=cb, error_handler=cb, dbus_interface='org.bluez.GattCharacteristic1')
    #propIf.WriteValue([1,2,3], {}, reply_handler=cb, error_handler=cb, dbus_interface='org.bluez.GattCharacteristic1', mainloop=mainloop)
    #propIf.WriteValue(dbus.Array(dbus.Byte(1)), {} )
    propIf.WriteValue([1,2,3], {} )
    #interface = dbus.Interface(chrObj, 'org.freedesktop.DBus.Properties')
    print('Done')


    #start_client()

    mainloop.run()


if __name__ == '__main__':
    main()
