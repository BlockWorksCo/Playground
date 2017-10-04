




import binascii
import sys


def WritePDU(hdlc, pdu):
    """
    """
    return hdlc+pdu



def WriteCS(hdlc):
    """
    """
    cs = 0xabcd

    return hdlc+('%04x'%cs)



def WriteLLC(hdlc, llc):
    """
    """
    return hdlc+('%06x'%llc)



def WriteFrameFormat(hdlc, frameType):
    """
    """
    frameLength = 0x11
    format  = (frameType<<8) | (frameLength&0x7ff)

    return hdlc+('%04x'%format)



def WriteFrameLength(hdlc):
    """
    11-bit field
    """
    frameLength = len(hdlc)
    frameLength = 0x22

    hdlc = hdlc[0:4] + ('%08x'%frameLength) + hdlc[6:]

    return hdlc



def WriteControlField(hdlc, control):
    """
    """
    return hdlc+('%02x'%control)



def WriteAddress(hdlc, address):
    """
    """
    if address <= 0xff:
        address = '%02x'%(address|0x01)

    if address <= 0xffff:
        address = '%04x'%(address|0x01)

    if address <= 0xffffff:
        address = '%08x'%(address|0x01)

    return hdlc+address


#hdlcHex = "7e a047 00020023 41 10 974b e6e600 6036a1090607608574050801018a0207808b0760857405080201ac0a80083132333435363738be10040e01000000065f1f0400001f1fffff dc52 7e"
#pduHex = "6036a1090607608574050801018a0207808b0760857405080201ac0a80083132333435363738be10040e01000000065f1f0400001f1fffff"
pduHex  = sys.argv[1]

print(pduHex)

controlField= 0x10
frameType   = 0xa0
length      = 0x00
srcAddress  = 0x41
dstAddress  = 0x00020023
LLC         = 0xe6e600

hdlc   = '7e'
hdlc   = WriteFrameFormat(hdlc,frameType)
hdlc   = WriteAddress(hdlc,dstAddress)
hdlc   = WriteAddress(hdlc,srcAddress)
hdlc   = WriteControlField(hdlc, controlField)
hdlc   = WriteCS(hdlc)
hdlc   = WriteLLC(hdlc, LLC)
hdlc   = WritePDU(hdlc, pduHex)
hdlc   = WriteFrameLength(hdlc)
hdlc   = WriteCS(hdlc)
hdlc   = hdlc+'7e'

print(hdlc)
