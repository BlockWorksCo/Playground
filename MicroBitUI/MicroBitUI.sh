#!/bin/sh

#gatttool -t random -b FD:8E:CC:FA:11:B8 --characteristics
#handle = 0x0002, char properties = 0x0a, char value handle = 0x0003, uuid = 00002a00-0000-1000-8000-00805f9b34fb
#handle = 0x0004, char properties = 0x02, char value handle = 0x0005, uuid = 00002a01-0000-1000-8000-00805f9b34fb
#handle = 0x0006, char properties = 0x02, char value handle = 0x0007, uuid = 00002a04-0000-1000-8000-00805f9b34fb
#handle = 0x0009, char properties = 0x20, char value handle = 0x000a, uuid = 00002a05-0000-1000-8000-00805f9b34fb
#handle = 0x000d, char properties = 0x20, char value handle = 0x000e, uuid = 6e400002-b5a3-f393-8877-665544332211
#handle = 0x0010, char properties = 0x0c, char value handle = 0x0011, uuid = 6e400003-b5a3-f393-8877-665544332211


gatttool -t random -b FD:8E:CC:FA:11:B8 --characteristics --uuid=6e400003-b5a3-f393-8877-665544332211

gatttool -t random -b FD:8E:CC:FA:11:B8 --characteristics --uuid=6e400003-b5a3-f393-8877-665544332211 | grep -oP "(?<=handle = 0x)[0-9a-f]+"

gatttool -t random -b FD:8E:CC:FA:11:B8 --handle=0x0011 --char-write-req --value==343536


#reading from Mio Alpha
gatttool -b xx:xx:xx:xx:xx:xx -t random --char-write-req -a 0x0025 -n 0100 --listen


gatttool -t random -b FD:8E:CC:FA:11:B8 --handle=0x000d --char-read
