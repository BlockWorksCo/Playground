#!/bin/sh


cat Examples/test.hdlc | xargs -n 1 -t ./pdu2xml.sh 1>>Test.result 2>>Test.result


