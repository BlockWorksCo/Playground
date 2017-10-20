#!/bin/sh

PDU=`java -jar xml2pdu.jar $1`
PDUToHDLC.py $PDU 


