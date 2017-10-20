#!/bin/sh


PDU=`HDLCToPDU.py $1`
java -jar pdu2xml.jar $PDU


