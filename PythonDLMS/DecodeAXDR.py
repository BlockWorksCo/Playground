





import inspect
import binascii
import sys
import struct

NONE        = 0x00
ARRAY       = 0x01
STRUCTURE   = 0x02
BOOLEAN     = 0x03
BIT_STRING  = 0x04
INT32       = 0x05
UINT32      = 0x06
OCTET_STRING= 0x09
STRING      = 0x0a
STRING_UTF8 = 0x0c
INT8        = 0x0f
UINT8       = 0x11
UINT16      = 0x12
UINT64      = 0x15
ENUM        = 0x16
FLOAT32     = 0x17
DATE        = 0x1a
TIME        = 0x1b


def Indent():
    """
    """
    return ' '*len(inspect.stack())


def ParseLength(pdu, position):
    """
    If MSB != 0, LS 7 bits contains num of following bytes which make up length
    If MSB == 0, byte contain length value
    """
    length = ord(pdu[position])
    position    = position + 1
    if length & 0x80 != 0:
        numberOfBytes   = length&0x7f
        length  = 0
        for i in range(numberOfBytes):
            length = length << 8 
            length = length | ord(pdu[position])
            position    = position + 1

    return length, position
    

def ParseArray(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    #length = ord(pdu[position])
    length,position = ParseLength(pdu, position)
    print('%s<Array length="%d" offset="%d">'%(Indent(),length,position)) 

    for i in range(length):
        print('%s<ArrayElement index="%d" offset="%d">'%(Indent(),i,position)) 
        position    = ParseField(pdu,position)
        print('%s</ArrayElement>'%(Indent())) 

    print('%s</Array>'%(Indent())) 
    return position



def DLMSDateTimeToText(dlmsDatetime):
    """
    """
    year        = int(dlmsDatetime[0:4], 16)
    month       = int(dlmsDatetime[4:6], 16)
    dayOfMonth  = int(dlmsDatetime[6:8], 16)
    dayOfWeek   = int(dlmsDatetime[8:10], 16)
    hour        = int(dlmsDatetime[10:12], 16)
    minute      = int(dlmsDatetime[12:14], 16)
    second      = int(dlmsDatetime[14:16], 16)
    hundreths   = int(dlmsDatetime[16:18], 16)
    deviation   = int(dlmsDatetime[18:22], 16)
    status      = int(dlmsDatetime[22:24], 16)
    text    =  "{}:{}:{}:{} {}/{}/{}".format(hour, minute, second, hundreths, dayOfMonth, month, year)
    return text


def ParseOctetString(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    length = ord(pdu[position])
    value   = pdu[position+1:position+1+length]
    value   = binascii.hexlify(value)
    if length == 0x0c:
        print('%s<OctetString offset="%d" datetime="%s">%s</OctetString>'%(Indent(),position,DLMSDateTimeToText(value),value))
    else:
        print('%s<OctetString offset="%d">%s</OctetString>'%(Indent(),position,value))

    return position+1+length

def ParseNone(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    length = ord(pdu[position])
    value   = pdu[position+1:position+1+length]
    value   = binascii.hexlify(value)
    print('%s<NoData offset="%d">%s</NoData>'%(Indent(),position,value))

    return position+1+length


def ParseStructure(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    numberOfFields = ord(pdu[position])
    print('%s<Structure length="%d" offset="%d">'%(Indent(),numberOfFields,position)) 

    position    += 1
    for i in range(numberOfFields):
        print('%s<Field index="%d" offset="%d">'%(Indent(),i,position)) 
        position    = ParseField(pdu,position)
        print('%s</Field>'%(Indent())) 

    print('%s</Structure>'%(Indent()))

    return position


def ParseUint8(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position])
    print('%s<Uint8 offset="%d">%02x</Uint8>'%(Indent(),position,value))

    return position+1


def ParseInt8(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position])
    print('%s<Int8 offset="%d">%02x</Int8>'%(Indent(),position,value))

    return position+1


def ParseBoolean(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position])
    print('%s<Boolean offset="%d">%02x</Boolean>'%(Indent(),position,value))

    return position+1


def ParseEnum(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position+0])
    print('%s<Enum offset="%d">%02x</Enum>'%(Indent(),position,value))

    return position+1


def ParseBitString(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    length = ord(pdu[position])/8
    value   = pdu[position:position+1+length]
    value   = binascii.hexlify(value)
    print('%s<BitString offset="%d">%s</BitString>'%(Indent(),position,value))

    return position+1+length


def ParseUint16(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    hi  = ord(pdu[position+0])
    lo  = ord(pdu[position+1])
    value   = (hi<<8)|lo
    print('%s<Uint16 offset="%d">%04x</Uint16>'%(Indent(),position,value))

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
    hexValue    = '%02x%02x%02x%02x'%(b0,b1,b2,b3)
    value   = struct.unpack('!f',binascii.unhexlify(hexValue) )[0]
    #print('%s<Float32 offset="%d">%1.10f</Float32>'%(Indent(),position,value))
    print('%s<Float32 offset="%d">%s</Float32>'%(Indent(),position,hexValue))

    return position+4


def ParseUint32(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    b0  = ord(pdu[position+0])
    b1  = ord(pdu[position+1])
    b2  = ord(pdu[position+2])
    b3  = ord(pdu[position+3])
    print('%s<Uint32 offset="%d">%02x%02x%02x%02x</Float32>'%(Indent(),position,b0,b1,b2,b3))

    return position+4


def ParseDate(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    b0  = ord(pdu[position+0])
    print('%s<Date offset="%d">%02x</Date>'%(Indent(),position,b0))

    return position+1


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
    elif tag == BIT_STRING:
        position    = ParseBitString(pdu,position)
    elif tag == UINT32:
        position    = ParseUint32(pdu,position)
    elif tag == DATE:
        position    = ParseDate(pdu,position)
    elif tag == NONE:
        #position    = ParseNone(pdu,position)
        #position    = position + 1
        pass
    else:
        print('unknown tag %02x'%tag)
        sys.exit(-1)

    return position


def ParseAXDR(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    print('%s<Data>'%(Indent())) 
    while position < len(pdu):
        position    = ParseField(pdu,position)
        
    print('%s</Data>'%(Indent())) 

    return position




if __name__ == '__main__':

    pduHex=open(sys.argv[1]).read()
    pdu     = binascii.unhexlify(pduHex.replace('\n',''))
    ParseAXDR(pdu,0)


