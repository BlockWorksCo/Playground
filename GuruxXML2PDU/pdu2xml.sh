#!/bin/sh


PDU=`python HDLCToPDU.py $1`
java -jar pdu2xml.jar $PDU


