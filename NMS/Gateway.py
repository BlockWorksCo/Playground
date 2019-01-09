#!/usr/bin/env python3



import socket
import ctypes
import base64
import binascii
import multiprocessing





downstreamBuffer    = {}
upstreamBuffer      = {}


class MessageHeader(ctypes.Structure):
    _pack_   = 1
    _fields_ = [('eui64', ctypes.c_byte*8),
                ('length', ctypes.c_int)]




def PacketReceiver():

    sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
    sock.bind( ('::1', 5092) )

    while True:
        data, addr = sock.recvfrom(4096)
        print( "received message from Agent:", data )




# Start off the PacketReceiver To receive packets from the Agents.
multiprocessing.Process( target=PacketReceiver ).start()

# Get downstream packages from the NMS and buffer them.
sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock.bind( ('::1', 5080) )

packageCount    = 0

while True:

    # Receive a package from the NMS.
    packageBase64, receivedFrom = sock.recvfrom(4096)

    print('package of %d bytes received.'%(len(packageBase64)))

    # Get the payload data
    package = base64.b64decode( packageBase64 )

    packetCount = 0

    # Parse all packets in the package.
    while len(package) > 0:

        # Parse the payload header
        header      = MessageHeader.from_buffer_copy( package )

        # Unpack this packet from the package.
        eui64           = binascii.hexlify(bytearray(header.eui64))
        packetSize      = header.length
        packetPayload   = package[ctypes.sizeof(MessageHeader):ctypes.sizeof(MessageHeader)+packetSize]
        print( "%d/%d received %d byte packet for device with EUI64: %s"%( packageCount, packetCount, len(packetPayload), eui64 ) )

        # Store this packet in the downstream buffer for collection by the Agent.
        try:
            downstreamBuffer[eui64].append(packetPayload)
        except KeyError:
            downstreamBuffer[eui64] = [packetPayload]

        # Move onto the next packet in the package
        package     = package[ctypes.sizeof(MessageHeader)+packetSize:]

        packetCount = packetCount + 1

    packageCount    = packageCount + 1
    
    for eui in downstreamBuffer.keys():
        print("[%s] %d"%(eui,len(downstreamBuffer[eui64])));

