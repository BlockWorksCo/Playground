#!/usr/bin/python

import dbus
import dbus.mainloop.glib
import bluezutils
import time
import sys




"""
    /org/bluez/hci0/dev_FD_8E_CC_FA_11_B8

[ FD:8E:CC:FA:11:B8 ]
    AddressType = random
    Name = BBC micro:bit [pegug]
    Alias = BBC micro:bit [pegug]
    ServicesResolved = 0
    Adapter = /org/bluez/hci0
    Appearance = 512
    LegacyPairing = 0
    Paired = 0
    Connected = 0
    UUIDs = dbus.Array([dbus.String(u'00001800-0000-1000-8000-00805f9b34fb'), dbus.String(u'00001801-0000-1000-8000-00805f9b34fb'), dbus.String(u'6e400001-b5a3-f393-8877-665544332211')], signature=dbus.Signature('s'), variant_level=1)
    Address = FD:8E:CC:FA:11:B8
    RSSI = -82
    Trusted = 0
    Blocked = 0


handle = 0x0002, char properties = 0x0a, char value handle = 0x0003, uuid = 00002a00-0000-1000-8000-00805f9b34fb
handle = 0x0004, char properties = 0x02, char value handle = 0x0005, uuid = 00002a01-0000-1000-8000-00805f9b34fb
handle = 0x0006, char properties = 0x02, char value handle = 0x0007, uuid = 00002a04-0000-1000-8000-00805f9b34fb
handle = 0x0009, char properties = 0x20, char value handle = 0x000a, uuid = 00002a05-0000-1000-8000-00805f9b34fb
handle = 0x000d, char properties = 0x2c, char value handle = 0x000e, uuid = 6e400002-b5a3-f393-8877-665544332211
handle = 0x0010, char properties = 0x12, char value handle = 0x0011, uuid = 6e400003-b5a3-f393-8877-665544332211

"""


def MyMain():

    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    bus = dbus.SystemBus()

    om = dbus.Interface(bus.get_object("org.bluez", "/"), "org.freedesktop.DBus.ObjectManager")

    objects = om.GetManagedObjects()

    device  = objects['/org/bluez/hci0/dev_FD_8E_CC_FA_11_B8']

    a = device['org.bluez.Device1']

    proxy = bus.get_object('org.bluez', '/org/bluez/hci0/dev_FD_8E_CC_FA_11_B8')
    pp=dbus.Interface(proxy, 'org.bluez.Device1')

    pp.Connect()

    
    # Read from the characteristic.
    characteristic=dbus.Interface(proxy, 'org.bluez.GattCharacteristic1')
    proxy2 = bus.get_object('org.bluez', '/org/bluez/hci0/dev_FD_8E_CC_FA_11_B8/service000c/char0010')
    pp2=dbus.Interface(proxy2, 'org.bluez.GattCharacteristic1')
    d = pp2.ReadValue( {} )
    #d = pp2.ReadValue( dbus.Dictionary({'offset': 0}, signature='sv') )
    print(d)
    for c in d:
        sys.stdout.write('%c'%(int(c)))
    
    # write to a characteristic
    characteristic=dbus.Interface(proxy, 'org.bluez.GattCharacteristic1')
    proxy2 = bus.get_object('org.bluez', '/org/bluez/hci0/dev_FD_8E_CC_FA_11_B8/service000c/char0010')
    pp2=dbus.Interface(proxy2, 'org.bluez.GattCharacteristic1')
    ba = dbus.ByteArray( [0x7e, 0x00, 0x07, 0x00, 0x11, 0x22, 0x33, 0x44, 0x7e] )
    o = dbus.Dictionary({'offset': 0}, signature='sv')
    d = pp2.WriteValue( ba, o )

    pp.Disconnect()



if __name__ == '__main__':

    MyMain()



