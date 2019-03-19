#!/usr/bin/python



import HDLCToPDU
import ParseAXDR
import sys
import pprint


if __name__ == '__main__':

    hexPDU  = sys.argv[1].rstrip()

    print(hexPDU)
    print()

    outerResult  = HDLCToPDU.ParseHDLCPDU( hexPDU )

    combinedResult      = outerResult
    if combinedResult['segmentationFlag'] == 0:

        # This is the entire frame.
        
        controlLS   =  combinedResult['bottomControlField']
        controlMS   =  combinedResult['topControlField']
        #print outerResult

        if controlLS & 0x01 == 0x00:
            # This is an I-Frame.
            innerResult  = ParseAXDR.ParseAXDR( outerResult['PDU'], 0 )
            combinedResult['PDU']  = innerResult

        else:
            # This is an S-Frame.
            # See Green book 8.4.3.1
            if controlLS == 0x01:
                frameType   = 'RR'
            elif controlLS == 0x05:
                frameType   = 'RNR'
            elif controlLS == 0x03 and controlMS == 0x80:
                frameType   = 'SNRM'
            elif controlLS == 0x03 and controlMS == 0x40:
                frameType   = 'DISC'
            elif controlLS == 0x03 and controlMS == 0x60:
                frameType   = 'UA'
            elif controlLS == 0x0f:
                frameType   = 'DM'
            elif controlLS == 0x07:
                frameType   = 'FRMR'
            elif controlLS == 0x03 and controlMS == 0x00:
                frameType   = 'FRMR'

            combinedResult['PDU']  = [(-1,frameType,outerResult['PDU'])]
            

    else:

        # This is a partial frame/segment.
        combinedResult['PDU']  = [(-1,'NextSegment',outerResult['PDU'])]
        

    print
    pprint.pprint(combinedResult)
    print


