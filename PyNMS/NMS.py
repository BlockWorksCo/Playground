"""
#define TLS_AUTHENTICATION "ECDHE-ECDSA"

#define TLS_ENCRYPTION "AES128-CCM-8"

#define TLS_CIPHER_SUITE TLS_AUTHENTICATION "-" TLS_ENCRYPTION

"""



# pip install Dtls


import ssl
from socket import socket, AF_INET, SOCK_DGRAM
from dtls import do_patch


do_patch()

# https://docs.python.org/3/library/ssl.html#ssl.SSLContext.set_ciphers
context = ssl.create_default_context()
context.set_ciphers('ECDHE-ECDSA-AES128-CCM8')

sock = socket(AF_INET, SOCK_DGRAM)
sslsock = ssl.wrap_socket(sock)
sslsock.connect(('omnireltest2.dev.cyanconnode.com', 5684))
sslsock.send('Hi there')




