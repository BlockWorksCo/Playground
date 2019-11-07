

openssl s_server -dtls1_2 -cert cert.pem -key key.pem -accept 4444 -psk abc123 -cipher PSK-AES128-CCM8


