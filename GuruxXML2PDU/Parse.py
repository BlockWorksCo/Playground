





import inspect
import binascii
import sys

ARRAY       = 0x01
STRUCTURE   = 0x02
BOOLEAN     = 0x03
STRING_UTF8 = 0x0b
OCTET_STRING= 0x09
UINT8       = 0x11
UINT16      = 0x12
INT8        = 0x0f
ENUM        = 0x16
FLOAT32     = 0x17



def Indent():
    """
    """
    return ' '*len(inspect.stack())


def ParseArray(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    length = ord(pdu[position])
    print('%04x:%sarray of length %d'%(position,Indent(),length)) 

    position    += 1
    for i in range(length):
        position    = ParseAXDR(pdu,position)

    return position


def ParseOctetString(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    length = ord(pdu[position])
    value   = pdu[position:position+1+length]
    value   = binascii.hexlify(value)
    print('%04x:%soctet string of length %d = [%s]'%(position,Indent(),length,value))

    return position+1+length


def ParseStructure(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    numberOfFields = ord(pdu[position])
    print('%04x:%sstructure with %d fields'%(position,Indent(),numberOfFields)) 

    position    += 1
    for i in range(numberOfFields):
        position    = ParseField(pdu,position)

    return position


def ParseUint8(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position])
    print('%04x:%sUINT8 of value %02x'%(position,Indent(),value))

    return position+1


def ParseInt8(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position])
    print('%04x:%sINT8 of value %02x'%(position,Indent(),value))

    return position+1


def ParseBoolean(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position])
    print('%04x:%sBOOLEAN of value %02x'%(position,Indent(),value))

    return position+1


def ParseEnum(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    hi  = ord(pdu[position+0])
    lo  = ord(pdu[position+1])
    value   = (hi<<8)|lo
    print('%04x:%sENUM of value %02x'%(position,Indent(),value))

    return position+3


def ParseUint16(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    hi  = ord(pdu[position+0])
    lo  = ord(pdu[position+1])
    value   = (hi<<8)|lo
    print('%04x:%sUINT16 of value %04x'%(position,Indent(),value))

    return position+2


def ParseFloat32(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    b0  = ord(pdu[position+0])
    b1  = ord(pdu[position+1])
    b2  = ord(pdu[position+2])
    b3  = ord(pdu[position+3])
    print('%04x:%sFLOAT32 of value %02x%02x%02x%02x'%(position,Indent(),b0,b1,b2,b3))

    return position+4


def ParseField(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    tag = ord(pdu[position])
    position    += 1

    if tag == ARRAY:
        position    = ParseArray(pdu,position)
    elif tag == STRUCTURE:
        position    = ParseStructure(pdu,position)
    elif tag == UINT8:
        position    = ParseUint8(pdu,position)
    elif tag == UINT16:
        position    = ParseUint16(pdu,position)
    elif tag == OCTET_STRING:
        position    = ParseOctetString(pdu,position)
    elif tag == INT8:
        position    = ParseInt8(pdu,position)
    elif tag == ENUM:
        position    = ParseEnum(pdu,position)
    elif tag == BOOLEAN:
        position    = ParseBoolean(pdu,position)
    elif tag == FLOAT32:
        position    = ParseFloat32(pdu,position)
    else:
        print('unknown tag %02x'%tag)
        sys.exit(-1)

    return position


def ParseAXDR(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    while position < len(pdu):
        position    = ParseField(pdu,position)
        
    return position



pduHex=open(sys.argv[1]).read()
pdu     = binascii.unhexlify(pduHex.replace('\n',''))

ParseAXDR(pdu,0)


