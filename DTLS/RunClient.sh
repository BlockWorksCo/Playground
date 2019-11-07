


openssl s_client -dtls1_2 -connect 127.0.0.1:4444 -debug -cert cert.pem -key key.pem -psk abc123 -cipher PSK-AES128-CCM8



