
#./main -i tun1 -s

ip add add fd00::1 dev tun1
ifconfig tun1 up
ip -6 route add fd00::1/64 dev tun1


nc -6 -u fd00::2 80



