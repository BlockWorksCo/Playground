#!/usr/bin/python



import HDLCToPDU
import ParseAXDR
import sys
import pprint







if __name__ == '__main__':

    hexPDU  = sys.argv[1]

    outerResult  = HDLCToPDU.ParseHDLCPDU( hexPDU )
    #print(result)

    innerResult  = ParseAXDR.ParseAXDR( outerResult['PDU'], 0 )
    #print(axdrResult)

    combinedResult      = outerResult
    combinedResult['PDU']  = innerResult

    #print(combinedResult)
    pprint.pprint(combinedResult)


