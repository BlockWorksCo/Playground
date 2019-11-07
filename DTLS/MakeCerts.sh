
#
#
#
openssl ecparam -out key.pem -name prime256v1 -genkey
openssl req -new -sha256 -key key.pem -out server.csr
openssl x509 -req -sha256 -days 365 -in server.csr -signkey key.pem -out cert.pem


