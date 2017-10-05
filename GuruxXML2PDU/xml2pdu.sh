#!/bin/sh

rm -f hdlcInfo

PDU=`java -jar xml2pdu.jar $1`

python PDUToHDLC.py $PDU > hdlcInfo
cat hdlcInfo


