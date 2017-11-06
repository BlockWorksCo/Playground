





import inspect
import binascii
import sys
import struct
import DecodeAXDR


def Indent():
    """
    """
    return ' '*len(inspect.stack())




def ParseGetRequestNormal(pdu, position):
    """
    """
    ic          = ord(pdu[position])
    position    += 1

    obis        = pdu[position:position+6]
    position    += 6
    
    attribute   = ord(pdu[position])
    position    += 1

    template    = \
    """
    <AttributeDescriptor>
      <ClassId Value="%04x" />
      <InstanceId Value="%08s" />
      <AttributeId Value="%02x" />
    </AttributeDescriptor>
    """

    xml = template%(ic,binascii.hexlify(obis),attribute)

    return xml,position



def ParseGetResponseNormal(pdu, position):
    """
    """
    ic          = ord(pdu[position])
    position    += 1

    obis        = pdu[position:position+6]
    position    += 6
    
    attribute   = ord(pdu[position])
    position    += 1

    accessSelector = ord(pdu[position])
    position    += 1

    template    = \
    """
    <AttributeDescriptor>
      <ClassId Value="%04x" />
      <InstanceId Value="%08s" />
      <AttributeId Value="%02x" />
    </AttributeDescriptor>
    """

    xml = template%(ic,binascii.hexlify(obis),attribute)

    return xml,position



def DecodeDLMS(pdu, position=0):
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

    xml         = ''

    type        = ord(pdu[position])
    position    += 1
    subType     = ord(pdu[position])
    position    += 1
    invokeId    = ord(pdu[position])
    position    += 1
    priority    = ord(pdu[position])
    position    += 1

    fullType    = type << 8 | subType

    tags    = \
    {
        0xc001:{'OpenTag':'<GetRequest><GetRequestNormal><InvokeIdAndPriority Value="%02x%02x" />'%(invokeId,priority), 'CloseTag':'</GetRequestNormal></GetRequest>'},
        0xc002:{'OpenTag':'<GetRequest><GetRequestwithDataBlock>', 'CloseTag':'</GetRequestNormal></GetRequest>'},
        0xc101:{'OpenTag':'<SetRequest><SetRequestNormal>', 'CloseTag':'</SetRequestNormal></SetRequest>'},
        0xc401:{'OpenTag':'<GetResponse><GetResponseNormal><InvokeIdAndPriority Value="%02x%02x" />'%(invokeId,priority), 'CloseTag':'</GetResponseNormal></GetResponse>'},
    }

    xml += tags[fullType]['OpenTag']
    
    if fullType == 0xc001:
        result,position = ParseGetRequestNormal(pdu, position)
        xml += result

    elif fullType == 0xc501:
        result,position = ParseGetResponseNormal(pdu, position)
        xml += result

    DecodeAXDR.DecodeAXDR(pdu, position)
    xml += tags[fullType]['CloseTag']

    return xml

    



if __name__ == '__main__':

    hex = open(sys.argv[1]).read().replace('\n','').replace('\r','')
    #ParseDLMS(binascii.unhexlify('C0018100070100630100FF0201010204020412000809060000010000FF0F02120000090C07E007170600000000000000090C07E00719010B0300000000000100'), 0)
    DecodeDLMS(binascii.unhexlify(hex))


