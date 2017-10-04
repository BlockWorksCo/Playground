




import binascii


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
    remoteLSAP = ord(hdlc[position])
    position    = position+1

    localLSAP   = ord(hdlc[position])
    position    = position+1

    llcQuality  = ord(hdlc[position])
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

    format  = (formatHi<<8) | formatLo

    frameType   = format&0xf000
    frameLength = format&0x7ff

    return (frameType,frameLength,position)



def ReadControlField(position, hdlc):
    """
    """
    control = ord(hdlc[position])
    position    = position+1

    return (control,position)



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

    return (address,position)


# 0  1    3 4 5 6  7  8  9 10 11
# 7e a047 00020023 41 10 974b e6e600 6036a1090607608574050801018a0207808b0760857405080201ac0a80083132333435363738be10040e01000000065f1f0400001f1fffff dc52 7e
hdlcHex = "7ea047000200234110974be6e6006036a1090607608574050801018a0207808b0760857405080201ac0a80083132333435363738be10040e01000000065f1f0400001f1fffffdc527e"
hdlc    = binascii.unhexlify(hdlcHex)

print(hdlcHex)

if ord(hdlc[0]) != 0x7e or ord(hdlc[-1]) != 0x7e:
    print('no HDLC framing detected.')
else:
    position                    = 1
    frameType,length,position   = ReadFrameFormat(position, hdlc)
    dstAddress,position         = ReadAddress(position, hdlc)
    srcAddress,position         = ReadAddress(position, hdlc)
    controlField,position       = ReadControlField(position, hdlc)
    HCS,position                = ReadCS(position, hdlc)
    LLC,position                = ReadLLC(position, hdlc)
    pdu,position                = ReadPDU(position, hdlc)
    FCS,position                = ReadCS(position, hdlc)

    print('controlField=%x'%(controlField))
    print('frameFormat=%x,%d'%(frameType,length))
    print('src=%x'%(srcAddress))
    print('dst=%x'%(dstAddress))
    print('LLC=%x'%(LLC))
    print('HCS=%x'%(HCS))
    print('PDU=%s'%(binascii.hexlify(pdu)))
    print('FCS=%x'%(FCS))





