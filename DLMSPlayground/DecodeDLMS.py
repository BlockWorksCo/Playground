#!/usr/bin/python



import HDLCToPDU
import ParseAXDR
import sys
import pprint


if __name__ == '__main__':

    hexPDU  = sys.argv[1]

    outerResult  = HDLCToPDU.ParseHDLCPDU( hexPDU )
    innerResult  = ParseAXDR.ParseAXDR( outerResult['PDU'], 0 )

    combinedResult      = outerResult
    combinedResult['PDU']  = innerResult

    pprint.pprint(combinedResult)


