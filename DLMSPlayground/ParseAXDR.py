





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

GLO_ACTIONREQUEST   = 0xcb
GLO_ACTIONRESPONSE  = 0xcf
GLO_GETREQUEST      = 0xc8
GET_REQUEST         = 0xc0
SET_REQUEST         = 0xc1
GET_RESPONSE        = 0xc4

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

    length,position = ParseLength(pdu, position)

    result  = []
    for i in range(length):
        position,typeString,value    = ParseField(pdu,position)
        result.append( (position,typeString,value) )

    return position,'Array-%d'%length,result



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

    return position+1+length,"OctetString-%d"%length,value

def ParseNone(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    length = ord(pdu[position])
    value   = pdu[position+1:position+1+length]
    value   = binascii.hexlify(value)

    return position+1+length,"NoData-%d"%length,value


def ParseStructure(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    numberOfFields = ord(pdu[position])

    result      = []
    position    += 1
    for i in range(numberOfFields):
        position,typeString,value    = ParseField(pdu,position)
        result.append( (position,typeString,value) )

    return position,"Structure-%d"%numberOfFields,result


def ParseUint8(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position])

    return position+1,"Uint8",value


def ParseInt8(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position])

    return position+1,"Int8",value


def ParseBoolean(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position])

    return position+1,"Boolean",value


def ParseEnum(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    value  = ord(pdu[position+0])

    return position+1,"Enumeration",value


def ParseBitString(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    length = ord(pdu[position])/8
    value   = pdu[position:position+1+length]
    value   = binascii.hexlify(value)

    return position+1+length,"BitString",value


def ParseUint16(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    hi  = ord(pdu[position+0])
    lo  = ord(pdu[position+1])
    value   = (hi<<8)|lo

    return position+2,"Uint16",value


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

    return position+4,"Float32",value


def ParseUint32(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    b0  = ord(pdu[position+0])
    b1  = ord(pdu[position+1])
    b2  = ord(pdu[position+2])
    b3  = ord(pdu[position+3])

    return position+4,"Uint32","%02x%02x%02x%02x"%(b0,b1,b2,b3)


def ParseDate(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    b0  = ord(pdu[position+0])
    print('%s<Date offset="%d">%02x</Date>'%(Indent(),position,b0))

    return position+1,"Date (TODO)",b0


def ParseField(pdu,position):
    """
    """
    if position == len(pdu):
        return position

    tag = ord(pdu[position])
    position    += 1
    typeString  = '?'
    value       = None

    if tag == ARRAY:
        position,typeString,value    = ParseArray(pdu,position)
    elif tag == STRUCTURE:
        position,typeString,value    = ParseStructure(pdu,position)
    elif tag == UINT8:
        position,typeString,value    = ParseUint8(pdu,position)
    elif tag == UINT16:
        position,typeString,value    = ParseUint16(pdu,position)
    elif tag == OCTET_STRING:
        position,typeString,value    = ParseOctetString(pdu,position)
    elif tag == INT8:
        position,typeString,value    = ParseInt8(pdu,position)
    elif tag == ENUM:
        position,typeString,value    = ParseEnum(pdu,position)
    elif tag == BOOLEAN:
        position,typeString,value    = ParseBoolean(pdu,position)
    elif tag == FLOAT32:
        position,typeString,value    = ParseFloat32(pdu,position)
    elif tag == BIT_STRING:
        position,typeString,value    = ParseBitString(pdu,position)
    elif tag == UINT32:
        position,typeString,value    = ParseUint32(pdu,position)
    elif tag == DATE:
        position,typeString,value    = ParseDate(pdu,position)
    elif tag == GLO_ACTIONREQUEST:
        typeString  = 'glo_ActionRequest'
        ppp         = pdu[position+1]
        value       = binascii.hexlify( pdu[position+1:] )
        position    = len(pdu)
    elif tag == GLO_ACTIONRESPONSE:
        typeString  = 'glo_ActionResponse'
        ppp         = pdu[position+1]
        value       = binascii.hexlify( pdu[position+1:] )
        position    = len(pdu)
    elif tag == GLO_GETREQUEST:
        typeString  = 'glo_GetRequest'
        ppp         = pdu[position+1]
        value       = binascii.hexlify( pdu[position+1:] )
        position    = len(pdu)
    elif tag == GET_REQUEST:
        subType     = pdu[position]
        invokeId    = pdu[position+1]
        priority    = pdu[position+2]
        classId     = pdu[position+3]
        obis        = pdu[position+4:position+10]
        attributeId = pdu[position+10]
        accessType  = pdu[position+11]
        typeString  = 'GetRequest'
        if ord(accessType) == 0:
            value       = (-1,'Structure-6', [(-1,'subType',ord(subType)), (-1,'invokeId',ord(invokeId)), (-1,'priority',ord(priority)), (-1,'classId',ord(classId)), (-1,'OBIS',binascii.hexlify(obis)), (-1,'attributeId',ord(attributeId)) ] )
            position    = position+12
        if ord(accessType) == 1:
            accessSelector = pdu[position+12]
            position,parameterTypeString,parameterValue  = ParseField( pdu,position+13  )
            value       = (-1,'Structure-6', [(-1,'subType',ord(subType)), (-1,'invokeId',ord(invokeId)), (-1,'priority',ord(priority)), (-1,'classId',ord(classId)), (-1,'OBIS',binascii.hexlify(obis)), (-1,'attributeId',ord(attributeId)), (-1,parameterTypeString,parameterValue) ] )
    elif tag == GET_RESPONSE:
        typeString  = 'GetResponse'
        subType     = pdu[position]
        invokeId    = pdu[position+1]
        priority    = pdu[position+2]
        position,nestedTypeString,nestedValue  = ParseField( pdu,position+3 )
        value       = (-1,'Structure-4', [(-1,'subType',ord(subType)), (-1,'invokeId',ord(invokeId)), (-1,'priority',ord(priority)), (-1,nestedTypeString,nestedValue) ] )
        position    = len(pdu)-1
    elif tag == NONE:
        #position    = ParseNone(pdu,position)
        #position    = position + 1
        pass
    else:
        return position,"UnknownTag",'%x'%tag

    return position,typeString,value


def ParseAXDR(pduHex,position):
    """
    """

    # convert from hex to binary
    pdu = binascii.unhexlify(pduHex.replace('\n',''))

    if position == len(pdu):
        return position

    result  = []
    while position < len(pdu):
        position,typeString,value    = ParseField(pdu,position)
        result.append( (position,typeString,value) )

    return result




def ParseDLMS(pdu, position):
    """
        'x', // Request tag
        0x01, 0x81, 0x00, // Type, subtype, invoke ID, priority
        'x', // Interface class
        'x', 'x', 'x', 'x', 'x', 'x', // OBIS code
        'x', // Attribute
        0x00, // Data

    7e a01a 00020023 41   10  1717 e6e600    c4   01      81   00  0905 3030303030 23a4 7e
            addr     add  ctl hcs  llc       type subtype invk pri data            fcs
    """
    if position == len(pdu):
        return position

    type        = ord(pdu[position])
    position    += 1
    subType     = ord(pdu[position])
    position    += 1
    invokeId    = ord(pdu[position])
    position    += 1
    priority    = ord(pdu[position])
    position    += 1

    fullType    = type << 8 | subType

    print('type = %02x'%type)
    print('subtype = %02x'%subType)
    print('invokeId = %02x'%invokeId)
    print('priority = %02x'%priority)
    print('fullType = %02x'%fullType)

    tags    = \
    {
        0xc001:{'OpenTag':'<GetRequest><GetRequestNormal>', 'CloseTag':'</GetRequestNormal></GetRequest>'},
        0xc002:{'OpenTag':'<GetRequest><GetRequestwithDataBlock>', 'CloseTag':'</GetRequestNormal></GetRequest>'},
        0xc101:{'OpenTag':'<SetRequest><SetRequestNormal>', 'CloseTag':'</SetRequestNormal></SetRequest>'},
    }

    print(tags[fullType]['OpenTag'])
    print(tags[fullType]['CloseTag'])

    return position





if __name__ == '__main__':

    pduHex=open(sys.argv[1]).read()
    pdu     = binascii.unhexlify(pduHex.replace('\n',''))
    ParseAXDR(pdu,0)

    #ParseDLMS(binascii.unhexlify('C0018100070100630100FF0201010204020412000809060000010000FF0F02120000090C07E007170600000000000000090C07E00719010B0300000000000100'), 0)


