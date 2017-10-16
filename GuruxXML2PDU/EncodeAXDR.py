


import xmltodict
import sys
import inspect


def Indent():
    """
    """
    return ' '*len(inspect.stack())



def EncodeAXDR(d):
    """
    """
    hexData = ''
    typeName    = type(d).__name__
    #print(typeName)
    
    if typeName == 'list' or typeName == 'dict' or typeName == 'OrderedDict':
        
        print('%svector type: [%s]'%(Indent(),str(d)))

        for key in d.keys():
            element = d[key]

            if key == 'Array':
                print('%sArray of %d elements'%(Indent(),len(d[key])))
                i   = 0
                for arrayElement in d[key]:
                    print('%sItem %d'%(Indent(),i))
                    hexData = EncodeAXDR(arrayElement)
                    i   = i+1 

            elif key == 'Data':
                print('%sData'%(Indent()))
                hexData = EncodeAXDR(d[key])

            elif key == 'ArrayElement':
                print('%sArrayElement'%(Indent()))
                hexData = EncodeAXDR(d[key])

            elif key == 'Structure':
                print('%sStructure of %d fields'%(Indent(),len(d[key])))
                i   = 0
                for element in d.items():
                    print('%sField %d'%(Indent(),i))
                    hexData = EncodeAXDR(element)
                    i   = i+1 

            else:
                print('Unknown element type [%s]'%key)

    else:
        if d[0] == '@':
            print('%signoring attribute %s'%(Indent(),d))
        else:
            if d == 'ArrayElement':
                print('%sArrayElement'%(Indent()))
                hexData = EncodeAXDR(d[key])

            print('%sScalar type [%s] is %s'%(Indent(),str(d),typeName))

    return hexData 



if __name__ == '__main__':
    """
    """
    xml     = open(sys.argv[1]).read()
    d       = xmltodict.parse(xml)
    hexPDU  = EncodeAXDR(d)
    print(hexPDU)

