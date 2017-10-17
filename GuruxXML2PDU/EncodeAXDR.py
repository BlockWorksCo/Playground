


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
        print('[in %s of type %s]'%(name, typeName))

        if typeName == 'dict' or typeName == 'OrderedDict':
            children    = d.keys()
            print('%d children %s (dict)'%(len(children),children))
            for child in children:
                EncodeAXDR(child, d[child])

        if typeName == 'list':
            print('%d children (list)'%(len(d)))
            i   = 0
            for child in d:
                EncodeAXDR(name+'_%d'%i, child)
                i   = i + 1

    return hexData 



if __name__ == '__main__':
    """
    """
    xml     = open(sys.argv[1]).read()
    d       = xmltodict.parse(xml)
    hexPDU  = EncodeAXDR('xml',d)
    print(hexPDU)

