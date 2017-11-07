





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


def ParseLength(pdu, position,xml):
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

    return length, position,xml
    

def ParseArray(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    #length = ord(pdu[position])
    length,position,xml = ParseLength(pdu, position, xml)
    xml += ('%s<Array length="%d" offset="%d">'%(Indent(),length,position)) 

    for i in range(length):
        xml += ('%s<ArrayElement index="%d" offset="%d">'%(Indent(),i,position)) 
        position,xml    = ParseField(pdu,position, xml)
        xml += ('%s</ArrayElement>'%(Indent())) 

    xml += ('%s</Array>'%(Indent())) 
    return position,xml



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


def ParseOctetString(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    length = ord(pdu[position])
    value   = pdu[position+1:position+1+length]
    value   = binascii.hexlify(value)
    if length == 0x0c:
        xml += ('%s<OctetString offset="%d" datetime="%s">%s</OctetString>'%(Indent(),position,DLMSDateTimeToText(value),value))
    else:
        xml += ('%s<OctetString offset="%d">%s</OctetString>'%(Indent(),position,value))

    return position+1+length,xml

def ParseNone(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    length = ord(pdu[position])
    value   = pdu[position+1:position+1+length]
    value   = binascii.hexlify(value)
    xml += ('%s<NoData offset="%d">%s</NoData>'%(Indent(),position,value))

    return position+1+length,xml


def ParseStructure(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    numberOfFields = ord(pdu[position])
    xml += ('%s<Structure length="%d" offset="%d">'%(Indent(),numberOfFields,position)) 

    position    += 1
    for i in range(numberOfFields):
        xml += ('%s<Field index="%d" offset="%d">'%(Indent(),i,position)) 
        position,xml    = ParseField(pdu,position, xml)
        xml += ('%s</Field>'%(Indent())) 

    xml += ('%s</Structure>'%(Indent()))

    return position,xml


def ParseUint8(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    value  = ord(pdu[position])
    xml += ('%s<Uint8 offset="%d">%02x</Uint8>'%(Indent(),position,value))

    return position+1,xml


def ParseInt8(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    value  = ord(pdu[position])
    xml += ('%s<Int8 offset="%d">%02x</Int8>'%(Indent(),position,value))

    return position+1,xml


def ParseBoolean(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    value  = ord(pdu[position])
    xml += ('%s<Boolean offset="%d">%02x</Boolean>'%(Indent(),position,value))

    return position+1,xml


def ParseEnum(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    value  = ord(pdu[position+0])
    xml += ('%s<Enum offset="%d">%02x</Enum>'%(Indent(),position,value))

    return position+1,xml


def ParseBitString(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    length = ord(pdu[position])/8
    value   = pdu[position:position+1+length]
    value   = binascii.hexlify(value)
    xml += ('%s<BitString offset="%d">%s</BitString>'%(Indent(),position,value))

    return position+1+length,xml


def ParseUint16(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    hi  = ord(pdu[position+0])
    lo  = ord(pdu[position+1])
    value   = (hi<<8)|lo
    xml += ('%s<Uint16 offset="%d">%04x</Uint16>'%(Indent(),position,value))

    return position+2,xml


def ParseFloat32(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    b0  = ord(pdu[position+0])
    b1  = ord(pdu[position+1])
    b2  = ord(pdu[position+2])
    b3  = ord(pdu[position+3])
    hexValue    = '%02x%02x%02x%02x'%(b0,b1,b2,b3)
    value   = struct.unpack('!f',binascii.unhexlify(hexValue) )[0]
    #xml += ('%s<Float32 offset="%d">%1.10f</Float32>'%(Indent(),position,value))
    xml += ('%s<Float32 offset="%d">%s</Float32>'%(Indent(),position,hexValue))

    return position+4,xml


def ParseUint32(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    b0  = ord(pdu[position+0])
    b1  = ord(pdu[position+1])
    b2  = ord(pdu[position+2])
    b3  = ord(pdu[position+3])
    xml += ('%s<Uint32 offset="%d">%02x%02x%02x%02x</Float32>'%(Indent(),position,b0,b1,b2,b3))

    return position+4,xml


def ParseDate(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    b0  = ord(pdu[position+0])
    xml += ('%s<Date offset="%d">%02x</Date>'%(Indent(),position,b0))

    return position+1,xml


def ParseField(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    tag = ord(pdu[position])
    position    += 1

    if tag == ARRAY:
        position,xml    = ParseArray(pdu,position, xml)
    elif tag == STRUCTURE:
        position,xml    = ParseStructure(pdu,position, xml)
    elif tag == UINT8:
        position,xml    = ParseUint8(pdu,position, xml)
    elif tag == UINT16:
        position,xml    = ParseUint16(pdu,position, xml)
    elif tag == OCTET_STRING:
        position,xml    = ParseOctetString(pdu,position, xml)
    elif tag == INT8:
        position,xml    = ParseInt8(pdu,position, xml)
    elif tag == ENUM:
        position,xml    = ParseEnum(pdu,position, xml)
    elif tag == BOOLEAN:
        position,xml    = ParseBoolean(pdu,position, xml)
    elif tag == FLOAT32:
        position,xml    = ParseFloat32(pdu,position, xml)
    elif tag == BIT_STRING:
        position,xml    = ParseBitString(pdu,position, xml)
    elif tag == UINT32:
        position,xml    = ParseUint32(pdu,position, xml)
    elif tag == DATE:
        position,xml    = ParseDate(pdu,position, xml)
    elif tag == NONE:
        #position    = ParseNone(pdu,position, xml)
        #position    = position + 1
        pass
    else:
        xml += ('unknown tag %02x'%tag)
        sys.exit(-1)

    return position,xml


def DecodeAXDR(pdu,position,xml):
    """
    """
    if position == len(pdu):
        return position,xml

    xml += ('%s<Data>'%(Indent())) 
    while position < len(pdu):
        position,xml    = ParseField(pdu,position,xml)
        
    xml += ('%s</Data>'%(Indent())) 

    return position,xml




if __name__ == '__main__':

    pduHex=open(sys.argv[1]).read()
    pdu     = binascii.unhexlify(pduHex.replace('\n',''))
    position,xml    = DecodeAXDR(pdu,0,'')
    print(xml)


