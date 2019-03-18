#!/usr/bin/python





import binascii
import sys
#import subprocess


def ReadPDU(position, hdlc):
    """
    """
    pdu = hdlc[position:-3]
    position    = position+len(pdu)

    return (pdu,position)



def ReadCS(position, hdlc):
    """
    """
    hi = ord(hdlc[position])
    position    = position+1

    lo = ord(hdlc[position])
    position    = position+1

    cs = (hi<<8) | lo

    return (cs,position)



def ReadLLC(position, hdlc):
    """
    """
    remoteLSAP = ord(hdlc[position])    # always 0xe6
    position    = position+1

    localLSAP   = ord(hdlc[position])   # always 0xe6 or 0xe7
    position    = position+1

    llcQuality  = ord(hdlc[position])   # always 0x00
    position    = position+1

    return (remoteLSAP,position)



def ReadFrameFormat(position, hdlc):
    """
    Note: this can be a two byte field if segmentation occurs.
    """
    formatHi = ord(hdlc[position])
    position    = position+1

    formatLo = ord(hdlc[position])
    position    = position+1

    # form the 16-bit field.
    format  = (formatHi<<8) | formatLo

    # Green book 8.4.1.3
    frameType           = format & 0xf000
    segmentationFlag    = format & 0x0800
    frameLength         = format & 0x07ff

    return (frameType,segmentationFlag,frameLength,position)



def ReadControlField(position, hdlc):
    """
    """
    control = ord(hdlc[position])
    position    = position+1

    # Green book 8.4.3.1
    pollFinalFlag   = control & 0x10
    iframeFlag      = control & 0x01
    topField        = control & 0xe0;
    bottomField     = control & 0x0e;

    return (pollFinalFlag,iframeFlag,topField,bottomField,position)



def ReadAddress(position, hdlc):
    """
    """
    address = ord(hdlc[position])
    position    = position+1

    if address&0x01 == 0:
        address     = (address << 8) | ord(hdlc[position])
        position    = position+1

    if address&0x01 == 0:
        address     = (address << 8) | ord(hdlc[position])
        position    = position+1

    if address&0x01 == 0:
        address     = (address << 8) | ord(hdlc[position])
        position    = position+1

    return (address>>1,position)


def ParseHDLCPDU(hdlcHex):

    # 0  1    3 4 5 6  7  8  9 10 11
    # 7e a047 00020023 41 10 974b e6e600 6036a1090607608574050801018a0207808b0760857405080201ac0a80083132333435363738be10040e01000000065f1f0400001f1fffff dc52 7e
    #hdlcHex = "7ea047000200234110974be6e6006036a1090607608574050801018a0207808b0760857405080201ac0a80083132333435363738be10040e01000000065f1f0400001f1fffffdc527e"
    hdlc    = binascii.unhexlify(hdlcHex)

    #print(hdlcHex)

    if ord(hdlc[0]) != 0x7e or ord(hdlc[-1]) != 0x7e:
        print('no HDLC framing detected.')
    else:
        position                    = 1
        frameType,segFlag,length,position   = ReadFrameFormat(position, hdlc)
        dstAddress,position         = ReadAddress(position, hdlc)
        srcAddress,position         = ReadAddress(position, hdlc)
        pollFinalFlag,iframeFlag,topField,bottomField,position       = ReadControlField(position, hdlc)
        HCS,position                = ReadCS(position, hdlc)
        LLC,position                = ReadLLC(position, hdlc)
        pdu,position                = ReadPDU(position, hdlc)
        FCS,position                = ReadCS(position, hdlc)

        result  = {}
        result['segmentationFlag']  = segFlag
        result['pollFinalFlag']  = pollFinalFlag
        result['iframeFlag']  = iframeFlag
        result['topControlField']  = topField
        result['bottomControlField']  = bottomField
        result['frameFormat']   = frameType
        result['frameLength']   = length
        result['srcAddress']    = srcAddress
        result['dstAddress']    = dstAddress
        result['LLC']           = LLC
        result['HCS']           = HCS
        result['PDU']           = binascii.hexlify(pdu)
        result['FCS']           = FCS

        return result

    return {}


