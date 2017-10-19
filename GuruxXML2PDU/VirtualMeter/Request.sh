#!/bin/sh


cat ../Examples/hdlc5.hdlc | ./unhexlify.py | netcat localhost 9999 | ./hexlify.py 

