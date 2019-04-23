#!/bin/bash -eu

##############################################################################
#
# Generate Certificates for PyDTLS Unit Testing
#
# This script is invoked manually (as opposed to by the unit test suite), in
# order to generate certain certificates that are required to be valid by
# the unit test suite.
#
# This script is not portable: it has been tested on Ubuntu 13.04 only. New
# certificates are written into the current directory.
#
# Copyright 2014 Ray Brown
#
##############################################################################

DIR=`dirname "$0"`

# Generate self-signed certificate for the certificate authority
echo Generating CA...; echo
openssl req -config "$DIR/openssl_ca.cnf" -x509 -newkey rsa -nodes -keyout ca-key.pem -out ca-cert.pem -days 3650

# Generate a server certificate request
echo Generating server certificate request...; echo
openssl req -config "$DIR/openssl_server.cnf" -newkey rsa -nodes -keyout server-key.pem -out server.csr

# Sign the request with the certificate authority's certificate created above
echo Signing server certificate request...; echo
openssl x509 -req -in server.csr -CA ca-cert.pem -CAkey ca-key.pem -CAcreateserial -days 3650 -out server-cert.pem

# Client Certificate
echo Generating client certificate request...; echo
openssl req -config "$DIR/openssl_server.cnf" -newkey rsa -nodes -keyout client-key.pem -out client.csr
 
# Sign the client CSR with the certificate authority's certificate created above
echo Signing server certificate request...; echo
openssl x509 -req -in client.csr -CA ca-cert.pem -CAkey ca-key.pem -CAcreateserial -days 3650 -out client-cert.pem


# Build pem file with private and public keys, ready for unprompted server use
cat server-key.pem server-cert.pem > keycert.pem


openssl x509 -outform der -in ca-cert.pem -out ca-cert.der
openssl x509 -outform der -in keycert.pem -out keycert.der
openssl x509 -outform der -in server-cert.pem -out server-cert.der




