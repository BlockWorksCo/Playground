


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

    if typeName == 'unicode':
        print('%sScalar type [%s] is %s'%(Indent(),str(d),typeName))
    else:
        print(d.keys())

        for key in d.keys():
            element = d[key]
            print('%sComposite %s %s'%(Indent(),key, typeName))

            if key == 'Array':
                print('%sArray of %d elements'%(Indent(),len(element)))
                i   = 0
                for arrayElement in element:
                    print('%sItem %d [%s]'%(Indent(),i,str(arrayElement)))
                    if arrayElement[0] == '@':
                        print('%sIgnoring [%s]'%(Indent(),str(d)))
                    else:
                        hexData = EncodeAXDR(d[arrayElement])
                    i   = i+1 

            elif key == 'Data':
                print('%sData'%(Indent()))
                hexData = EncodeAXDR(d[key])

            elif key == 'ArrayElement':
                print('%s->ArrayElement'%(Indent()))
                hexData = EncodeAXDR(d[key])

            elif key == 'Structure':
                print('%sStructure of %d fields'%(Indent(),len(d[key])))
                i   = 0
                for element in d.items():
                    print('%sField %d'%(Indent(),i))
                    hexData = EncodeAXDR(element)
                    i   = i+1 


    return hexData 



if __name__ == '__main__':
    """
    """
    xml     = open(sys.argv[1]).read()
    d       = xmltodict.parse(xml)
    hexPDU  = EncodeAXDR(d)
    print(hexPDU)

