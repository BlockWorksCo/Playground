#!/usr/bin/python

from __future__ import absolute_import, print_function, unicode_literals

import dbus
import dbus.mainloop.glib
import gobject as GObject

devices = {}

SERVICE_NAME = "org.bluez"
ADAPTER_INTERFACE = SERVICE_NAME + ".Adapter1"
DEVICE_INTERFACE = SERVICE_NAME + ".Device1"

def get_managed_objects():
	bus = dbus.SystemBus()
	manager = dbus.Interface(bus.get_object("org.bluez", "/"),
				"org.freedesktop.DBus.ObjectManager")
	return manager.GetManagedObjects()


def find_adapter(pattern=None):
	return find_adapter_in_objects(get_managed_objects(), pattern)


def find_adapter_in_objects(objects, pattern=None):
	bus = dbus.SystemBus()
	for path, ifaces in objects.iteritems():
		adapter = ifaces.get(ADAPTER_INTERFACE)
		if adapter is None:
			continue
		if not pattern or pattern == adapter["Address"] or \
							path.endswith(pattern):
			obj = bus.get_object(SERVICE_NAME, path)
			return dbus.Interface(obj, ADAPTER_INTERFACE)
	raise Exception("Bluetooth adapter not found")



def print_normal(address, properties):

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



def skip_dev(old_dev, new_dev):
    if not "Logged" in old_dev:
        return False
    if "Name" in old_dev:
        return True
    if not "Name" in new_dev:
        return True
    return False



def interfaces_added(path, interfaces):
    properties = interfaces["org.bluez.Device1"]
    if not properties:
        return

    if path in devices:
        dev = devices[path]
        devices[path] = dict(devices[path].items() + properties.items())
    else:
        devices[path] = properties

    if "Address" in devices[path]:
        address = properties["Address"]
    else:
        address = "<unknown>"

    if '6e400001-b5a3-f393-e0a9-e50e24dcca9e' in devices[path]['UUIDs']:
        print('<added>')
        print_normal(address, devices[path])



def properties_changed(interface, changed, invalidated, path):
    if interface != "org.bluez.Device1":
        return

    if path in devices:
        dev = devices[path]
        devices[path] = dict(devices[path].items() + changed.items())
    else:
        devices[path] = changed

    if "Address" in devices[path]:
        address = devices[path]["Address"]
    else:
        address = "<unknown>"

    if '6e400001-b5a3-f393-e0a9-e50e24dcca9e' in devices[path]['UUIDs']:
        print('<changed>')
        print_normal(address, devices[path])



if __name__ == '__main__':
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    bus = dbus.SystemBus()

    adapter = find_adapter(None)

    bus.add_signal_receiver(interfaces_added,
            dbus_interface = "org.freedesktop.DBus.ObjectManager",
            signal_name = "InterfacesAdded")

    bus.add_signal_receiver(properties_changed,
            dbus_interface = "org.freedesktop.DBus.Properties",
            signal_name = "PropertiesChanged",
            arg0 = "org.bluez.Device1",
            path_keyword = "path")

    om = dbus.Interface(bus.get_object("org.bluez", "/"),
                "org.freedesktop.DBus.ObjectManager")
    objects = om.GetManagedObjects()
    for path, interfaces in objects.iteritems():
        if "org.bluez.Device1" in interfaces:
            devices[path] = interfaces["org.bluez.Device1"]

    adapter.SetDiscoveryFilter({})
    adapter.StartDiscovery()

    mainloop = GObject.MainLoop()
    mainloop.run()
