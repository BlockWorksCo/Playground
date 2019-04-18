


# pip install Dtls


import ssl
from socket import socket, AF_INET, SOCK_DGRAM
from dtls import do_patch


do_patch()
sock = ssl.wrap_socket(socket(AF_INET, SOCK_DGRAM))
sock.connect(('172.16.32.106', 5684))
sock.send('Hi there')




