#!/usr/bin/python

import dbus
import dbus.mainloop.glib
import bluezutils
import time




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

    
    characteristic=dbus.Interface(proxy, 'org.bluez.GattCharacteristic1')
    proxy2 = bus.get_object('org.bluez', '/org/bluez/hci0/dev_FD_8E_CC_FA_11_B8/service000c/char0010')
    pp2=dbus.Interface(proxy2, 'org.bluez.GattCharacteristic1')
    d = pp2.ReadValue({})
    print(d)
    for c in d:
        print('%c'%(int(c)))
    

    pp.Disconnect()



if __name__ == '__main__':

    MyMain()



