#!/usr/bin/python



import HDLCToPDU
import ParseAXDR
import sys
import pprint


if __name__ == '__main__':

    hexPDU  = sys.argv[1]

    outerResult  = HDLCToPDU.ParseHDLCPDU( hexPDU )

    combinedResult      = outerResult
    if combinedResult['segmentationFlag'] == 0:
        innerResult  = ParseAXDR.ParseAXDR( outerResult['PDU'], 0 )
        combinedResult['PDU']  = innerResult
    else:
        combinedResult['PDU']  = [(-1,'NextSegment',outerResult['PDU'])]
        

    pprint.pprint(combinedResult)


