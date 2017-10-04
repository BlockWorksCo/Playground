#!/bin/sh


rm -f hdlcInfo
python HDLCToPDU.py $1 > hdlcInfo
cat hdlcInfo
PDU=`grep -oP "PDU=\K(.*)" hdlcInfo`
java -jar pdu2xml.jar $PDU


