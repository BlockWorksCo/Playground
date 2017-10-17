


import xmltodict
import sys


def EncodeAXDR(name,d):
    """
    """
    hexData = ''

    if name[0] == '@':
        pass

    else:
        typeName    = type(d).__name__

        if name == 'OctetString':
            textHex = d['#text']
            hexData = '09%02x%s'%(len(textHex)/2, textHex)

        elif name == 'Int8':
            hexData = '0f%s'%(d['#text'])

        elif name == 'Uint16':
            hexData = '12%s'%(d['#text'])

        elif name == 'Enum':
            hexData = '16%s'%(d['#text'])

        elif name == 'Uint32':
            hexData = '06%s'%(d['#text'])

        elif name == 'Uint8':
            hexData = '11%s'%(d['#text'])

        elif name == 'Uint64':
            hexData = '15%s'%(d['#text'])

        elif name == 'Float32':
            hexData = '17%s'%(d['#text'])

        elif name == 'Time':
            hexData = '1b%s'%(d['#text'])

        else:
            if typeName == 'dict' or typeName == 'OrderedDict':
                children    = d.keys()
                for child in children:
                    hexData = hexData + EncodeAXDR(child, d[child])

            if typeName == 'list':
                if name == 'ArrayElement':
                    hexData = '01%02x'%(len(d))
                if name == 'Field' or name == 'Field':
                    hexData = '02%02x'%(len(d))
                for child in d:
                    hexData = hexData + EncodeAXDR(' ', child)

    return hexData 



if __name__ == '__main__':
    """
    """
    xml     = open(sys.argv[1]).read()
    d       = xmltodict.parse(xml)
    hexPDU  = EncodeAXDR(' ',d)
    print(hexPDU)

