





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

    ParseDLMS(binascii.unhexlify('C0018100070100630100FF0201010204020412000809060000010000FF0F02120000090C07E007170600000000000000090C07E00719010B0300000000000100'), 0)


