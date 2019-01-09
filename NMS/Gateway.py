#!/usr/bin/env python3



import socket






sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock.bind( ('::1', 5080) )

while True:
    data, addr = sock.recvfrom(1024)
    print( "received message:", data )

