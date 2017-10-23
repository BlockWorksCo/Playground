#!/bin/sh


PATH=$PATH:.
PDU=`HDLCToPDU.py $1`
java -jar pdu2xml.jar $PDU


