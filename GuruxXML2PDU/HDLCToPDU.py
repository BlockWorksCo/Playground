




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



def ReadFrameFormat(position, hdlc):
    """
    Note: this can be a two byte field if segmentation occurs.
    """
    format = ord(hdlc[position])
    position    = position+1

    return (format,position)



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
        position    = position+1
        address     = (address << 8) | ord(hdlc[position])

    if address&0x01 == 0:
        position    = position+1
        address     = (address << 8) | ord(hdlc[position])

    if address&0x01 == 0:
        position    = position+1
        address     = (address << 8) | ord(hdlc[position])

    return (address,position)



# 7e a0 47 00020023 4110 974be6e6006036a1090607608574050801018a0207808b0760857405080201ac0a80083132333435363738be10040e01000000065f1f0400001f1fffff dc52 7e
hdlcHex = "7ea047000200234110974be6e6006036a1090607608574050801018a0207808b0760857405080201ac0a80083132333435363738be10040e01000000065f1f0400001f1fffffdc527e"
hdlc    = binascii.unhexlify(hdlcHex)

if ord(hdlc[0]) != 0x7e or ord(hdlc[-1]) != 0x7e:
    print('no HDLC framing.')
else:
    position                = 1
    frameFormat,position    = ReadFrameFormat(position, hdlc)
    dstAddress,position     = ReadAddress(position, hdlc)
    srcAddress,position     = ReadAddress(position, hdlc)
    controlField,position   = ReadControlField(position, hdlc)
    HCS,position            = ReadCS(position, hdlc)
    pdu,position            = ReadPDU(position, hdlc)
    FCS,position            = ReadCS(position, hdlc)

    print('controlField=%x'%(controlField))
    print('frameFormat=%x'%(frameFormat))
    print('src=%x'%(srcAddress))
    print('dst=%x'%(dstAddress))
    print('HCS=%x'%(HCS))
    print('PDU=%s'%(binascii.hexlify(pdu)))
    print('FCS=%x'%(FCS))





