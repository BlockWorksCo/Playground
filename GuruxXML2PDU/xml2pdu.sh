#!/bin/sh

rm -f pdu
rm -f hdlcInfo

java -jar xml2pdu.jar $1 > pdu
PDU=`grep -oP "PDU is \K(.*)" pdu`

echo $PDU
python PDUToHDLC.py $PDU > hdlcInfo
cat hdlcInfo


