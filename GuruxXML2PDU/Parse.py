





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



def Indent():
    """
    """
    return ' '*len(inspect.stack())


def ParseArray(pdu):
    """
    """
    length = ord(pdu[0])
    print('%sarray of length %d'%(Indent(),length)) 

    position    = 1
    for i in range(length):
        position    += ParseAXDR(pdu[position:])

    return position


def ParseOctetString(pdu):
    """
    """
    length = ord(pdu[0])
    value   = pdu[1:1+length]
    value   = binascii.hexlify(value)
    print('%soctet string of length %d = [%s]'%(Indent(),length,value))

    return 1+length


def ParseStructure(pdu):
    """
    """
    numberOfFields = ord(pdu[0])
    print('%sstructure with %d fields'%(Indent(),numberOfFields)) 

    position    = 1
    for i in range(numberOfFields):
        position    += ParseAXDR(pdu[position:])

    return position


def ParseUint8(pdu):
    """
    """
    value  = ord(pdu[0])
    print('%sUINT8 of value %02x'%(Indent(),value))

    return 1


def ParseInt8(pdu):
    """
    """
    value  = ord(pdu[0])
    print('%sINT8 of value %02x'%(Indent(),value))

    return 1


def ParseBoolean(pdu):
    """
    """
    value  = ord(pdu[0])
    print('%sBOOLEAN of value %02x'%(Indent(),value))

    return 1


def ParseEnum(pdu):
    """
    """
    hi  = ord(pdu[0])
    lo  = ord(pdu[1])
    value   = (hi<<8)|lo
    print('%sENUM of value %02x'%(Indent(),value))

    return 3


def ParseUint16(pdu):
    """
    """
    hi  = ord(pdu[0])
    lo  = ord(pdu[1])
    value   = (hi<<8)|lo
    print('%sUINT16 of value %04x'%(Indent(),value))

    return 2


def ParseAXDR(pdu):
    """
    """
    position    = 0
    while position < len(pdu):
        
        tag = ord(pdu[position])
        position    += 1

        if tag == ARRAY:
            position    += ParseArray(pdu[position:])
        elif tag == STRUCTURE:
            position    += ParseStructure(pdu[position:])
        elif tag == UINT8:
            position    += ParseUint8(pdu[position:])
        elif tag == UINT16:
            position    += ParseUint16(pdu[position:])
        elif tag == OCTET_STRING:
            position    += ParseOctetString(pdu[position:])
        elif tag == INT8:
            position    += ParseInt8(pdu[position:])
        elif tag == ENUM:
            position    += ParseEnum(pdu[position:])
        elif tag == BOOLEAN:
            position    += ParseBoolean(pdu[position:])
        else:
            print('unknown tag %02x'%tag)
            sys.exit(-1)

    return position



pduHex=open('object_list').read()
pdu     = binascii.unhexlify(pduHex.replace('\n',''))

ParseAXDR(pdu)


