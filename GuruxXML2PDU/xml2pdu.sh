#!/bin/sh

PDU=`java -jar xml2pdu.jar $1`
python PDUToHDLC.py $PDU 


