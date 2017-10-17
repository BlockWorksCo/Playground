


import xmltodict
import sys
import inspect


def Indent():
    """
    """
    return ' '*len(inspect.stack())



def EncodeAXDR(name,d):
    """
    """
    hexData = ''

    if name[0] == '@':
        #print('...ignoring')
        pass

    else:
        typeName    = type(d).__name__
        #print('[in %s of type %s]'%(name, typeName))

        if name == 'OctetString':
            hexData = ('OctetString [%s]'%(d['#text']))

        elif name == 'Int8':
            hexData = ('Int8 [%s]'%(d['#text']))

        elif name == 'Uint16':
            hexData = ('Uint16 [%s]'%(d['#text']))

        else:
            if typeName == 'dict' or typeName == 'OrderedDict':
                children    = d.keys()
                if name == 'ArrayElement' or name == 'Field':
                    hexData = '%s of %d :'%(name,len(children))
                for child in children:
                    hexData = hexData + EncodeAXDR(child, d[child])

            if typeName == 'list':
                i   = 0
                if name == 'ArrayElement' or name == 'Field':
                    hexData = '%s of %d :'%(name,len(d))
                for child in d:
                    hexData = hexData + EncodeAXDR(' ', child)
                    i   = i + 1

    return hexData 



if __name__ == '__main__':
    """
    """
    xml     = open(sys.argv[1]).read()
    d       = xmltodict.parse(xml)
    hexPDU  = EncodeAXDR(' ',d)
    print(hexPDU)

