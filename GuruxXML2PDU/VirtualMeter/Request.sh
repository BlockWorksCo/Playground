#!/bin/sh


cat $1 | ./unhexlify.py | netcat localhost 9999 | ./hexlify.py > temp.pdu
PYTHONPATH=.. PATH=$PATH:.. ../pdu2xml.sh `cat temp.pdu`

