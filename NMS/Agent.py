#!/usr/bin/env python3



import socket
import time
import binascii
import ctypes
import base64
import multiprocessing






def PacketReceiver():

    sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
    sock.bind( ('::1', 5091) )

    while True:
        data, addr = sock.recvfrom(4096)
        print( "received message from Gateway:", data )




multiprocessing.Process( target=PacketReceiver ).start()


# Socket for sending upstream packets to the Gateway
dataSocket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)

while(True):

    time.sleep( 60.0 )
    print('** Wakeup **')

    data    = b'' 
    dataSocket.sendto( data, ("::1", 5092) )

