#!/usr/bin/python

from __future__ import absolute_import, print_function, unicode_literals

import dbus
import dbus.mainloop.glib
import gobject as GObject
from flask import Flask, render_template, send_from_directory, request
import syslog
import multiprocessing





class Server(object):
    """ """

    def __init__(self, scanner):
        self.app = Flask(__name__)
        self.scanner    = scanner
        self.app.add_url_rule('/Index','index',self.Index)
        self.app.add_url_rule('/Main','main',self.main)
        p = multiprocessing.Process(target=self.app.run, args=(None,))
        p.start()


    def Index(self,):
        print('Index')
        return 'Index'

    def main(self,):
        return "Welcome! [%s]"%self.scanner.devices





class Scanner:
    """ """

    devices = {}

    ADAPTER_INTERFACE = "org.bluez.Adapter1"
    DEVICE_INTERFACE = "org.bluez.Device1"



    def __init__(self):
        dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

        bus = dbus.SystemBus()

        adapter = self.find_adapter(None)

        bus.add_signal_receiver(self.interfaces_added,
                dbus_interface = "org.freedesktop.DBus.ObjectManager",
                signal_name = "InterfacesAdded")

        bus.add_signal_receiver(self.properties_changed,
                dbus_interface = "org.freedesktop.DBus.Properties",
                signal_name = "PropertiesChanged",
                arg0 = "org.bluez.Device1",
                path_keyword = "path")

        om = dbus.Interface(bus.get_object("org.bluez", "/"),
                    "org.freedesktop.DBus.ObjectManager")
        objects = om.GetManagedObjects()
        for path, interfaces in objects.iteritems():
            if "org.bluez.Device1" in interfaces:
                self.devices[path] = interfaces["org.bluez.Device1"]

        adapter.SetDiscoveryFilter({})
        adapter.StartDiscovery()


    def get_managed_objects(self):
        bus = dbus.SystemBus()
        manager = dbus.Interface(bus.get_object("org.bluez", "/"),
                    "org.freedesktop.DBus.ObjectManager")
        return manager.GetManagedObjects()


    def find_adapter(self, pattern=None):
        return self.find_adapter_in_objects(self.get_managed_objects(), pattern)


    def find_adapter_in_objects(self, objects, pattern=None):
        bus = dbus.SystemBus()
        for path, ifaces in objects.iteritems():
            adapter = ifaces.get(self.ADAPTER_INTERFACE)
            if adapter is None:
                continue
            if not pattern or pattern == adapter["Address"] or \
                                path.endswith(pattern):
                obj = bus.get_object('org.bluez', path)
                return dbus.Interface(obj, self.ADAPTER_INTERFACE)
        raise Exception("Bluetooth adapter not found")



    def print_normal(self, address, properties):

        print("[ " + address + " ]")

        for key in properties.keys():
            value = properties[key]
            if type(value) is dbus.String:
                value = unicode(value).encode('ascii', 'replace')
            if (key == "Class"):
                print("    %s = 0x%06x" % (key, value))
            else:
                print("    %s = %s" % (key, value))

        print()

        properties["Logged"] = True



    def interfaces_added(self, path, interfaces):
        if 'org.bluez.Device1' in interfaces.keys():
            properties = interfaces["org.bluez.Device1"]
            if not properties:
                return
        else:
            print('<not a bluez interface>')
            return

        if path in self.devices:
            dev = self.devices[path]
            self.devices[path] = dict(self.devices[path].items() + properties.items())
        else:
            self.devices[path] = properties

        if "Address" in self.devices[path]:
            address = properties["Address"]
        else:
            address = "<unknown>"

        if '6e400001-b5a3-f393-e0a9-e50e24dcca9e' in self.devices[path]['UUIDs']:
            print('<added>')
            self.print_normal(address, self.devices[path])



    def properties_changed(self, interface, changed, invalidated, path):
        if interface != "org.bluez.Device1":
            return

        if path in self.devices:
            dev = self.devices[path]
            self.devices[path] = dict(self.devices[path].items() + changed.items())
        else:
            self.devices[path] = changed

        if "Address" in self.devices[path]:
            address = self.devices[path]["Address"]
        else:
            address = "<unknown>"

        if '6e400001-b5a3-f393-e0a9-e50e24dcca9e' in self.devices[path]['UUIDs']:
            print('<changed>')
            self.print_normal(address, self.devices[path])



if __name__ == '__main__':

    scanner = Scanner()
    server  = Server(scanner)

    mainloop = GObject.MainLoop()
    mainloop.run()


