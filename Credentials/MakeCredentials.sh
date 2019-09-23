#!/bin/bash

set -ex


#
# Create DTLS keys and certs.
#

CURVE=prime256v1
 
echo ---- Create CA key and self-signed cert ----
openssl ecparam -out ca.key -name ${CURVE} -genkey
#create self signed root cert
openssl req -new -key ca.key -x509 -nodes -days 3650 -out ca.crt -subj "/C=UK/ST=Cambridge/L=Cambridge/O=CyanConnode/OU=R&D/CN=c4.cyanconnode.com"
# Create DER equivalents of PEM
openssl x509 -inform PEM -in ca.crt -outform DER -out ca.crt.der
openssl ec -inform PEM -in ca.key -outform DER -out ca.key.der

echo ---- Create server key and sign with CA ----
openssl ecparam -out server.key -name ${CURVE} -genkey
#generate server CSR
openssl req -new -key server.key -out server.csr -subj "/C=UK/ST=Cambridge/L=Cambridge/O=CyanConnode/OU=R&D/CN=internal.cyanconnode.com"
#sign server CSR to create server.crt
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 3650
# Create DER equivalents of PEM
openssl x509 -inform PEM -in server.crt -outform DER -out server.crt.der
openssl ec -inform PEM -in server.key -outform DER -out server.key.der

echo ---- Create client key and sign with CA ----
openssl ecparam -out node.key -name ${CURVE} -genkey
#generate client CSR
openssl req -new -key node.key -out node.csr -subj "/CN=00-00-00-00-00-00-00-01"
#sign client CSR to create node.crt
openssl x509 -req -in node.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out node.crt -days 3650
# Create DER equivalents of PEM
openssl x509 -inform PEM -in node.crt -outform DER -out node.crt.der
openssl ec -inform PEM -in node.key -outform DER -out node.key.der


#
# Server security pair (db access).
#

echo ---- Generate server security pair ----
openssl genrsa -out ServerSecurityPair.pem 2048
openssl rsa -in ServerSecurityPair.pem -pubout | tee ServerSecurityPair.pem.pub





#
# Consistency checks
#

echo ---- Check node crt is signed by CA.crt ----
openssl verify -verbose -CAfile ca.crt  node.crt
 
echo ---- Check server crt is signed by CA.crt ----
openssl verify -verbose -CAfile ca.crt  server.crt

echo ---- Check that node key matches crt. All hashes should match ----
openssl pkey -in node.key -pubout -outform pem | sha256sum 
openssl x509 -in node.crt -pubkey -noout -outform pem | sha256sum 
openssl req -in node.csr -pubkey -noout -outform pem | sha256sum
 
echo ---- Check that server key matches crt. All hashes should match ----
openssl pkey -in server.key -pubout -outform pem | sha256sum 
openssl x509 -in server.crt -pubkey -noout -outform pem | sha256sum 
openssl req -in server.csr -pubkey -noout -outform pem | sha256sum
 
echo ---- Check that CA key matches crt. All hashes should be the same ----
openssl pkey -in ca.key -pubout -outform pem | sha256sum 
openssl x509 -in ca.crt -pubkey -noout -outform pem | sha256sum 


#
# Insert into keystore for server to use.
#
 
#create keystore
#keytool -import -keystore server.keystore -storepass "Connode" -file root.crt -alias client -trustcacerts
#openssl pkcs12 -export -in server.crt -inkey server.key -out server.p12 -name server -CAfile root.crt -caname root
#keytool -importkeystore -deststorepass Connode -destkeypass Connode -destkeystore server.keystore -srckeystore server.p12 -srcstoretype PKCS12 -srcstorepass Connode -alias server
 
# generate CA key
#openssl ecparam -out ca.key -name prime256v1 -genkey
#openssl req -new -key ca.key -x509 -nodes -days 3650 -out ca.crt -subj '/C=UK/ST=Cambridge/L=Cambridge/O=CyanConnode/OU=R&D/CN=internal.cyanconnode.com'

# convert PEM to DER
#openssl x509 -in ca.crt -outform DER -out ca.crt.der


# generate node key
#openssl req -new -nodes -out node.csr -newkey ec:ca.crt -keyout node.key -subj '/CN=00-00-00-00-00-00-00-01'
#openssl x509 -req -in node.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out node.crt.der -outform DER -sha256 -days 3650

# convert PEM to DER format.
#openssl x509 -inform DER -in node.crt.der -outform PEM -out node.crt
#openssl ec -in node.key -outform DER -out node.key.der



