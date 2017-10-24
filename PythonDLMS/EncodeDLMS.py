

import xmltodict
from collections import *
import sys
import EncodeAXDR



def EncodeDLMS(d):
    """
    """
    majorType   = d.keys()[0]
    minorType   = d[majorType].keys()[0]

    if minorType == 'GetRequestNormal':
        fullType    = 0xc001
        invokeIDAndPriority = int(d[majorType][minorType]['InvokeIdAndPriority']['@Value'],16)
        ic          = int(d[majorType][minorType]['AttributeDescriptor']['ClassId']['@Value'], 16)
        obis        = d[majorType][minorType]['AttributeDescriptor']['InstanceId']['@Value']
        attributeId = int(d[majorType][minorType]['AttributeDescriptor']['AttributeId']['@Value'],16)
        data        = d[majorType][minorType]['Data']
        dataPDU     = EncodeAXDR.EncodeAXDR(' ',data)
        invokeID    = invokeIDAndPriority&0xff
        priority    = (invokeIDAndPriority&0xff00)>>8
        pdu         = '%04x%02x%02x%02x%s%02x%s'%(fullType,invokeID,priority,ic,obis,attributeId,dataPDU)
    elif minorType == 'GetResponseNormal':
        fullType    = 0xc401
        invokeIDAndPriority = int(d[majorType][minorType]['InvokeIdAndPriority']['@Value'],16)
        data        = d[majorType][minorType]['Data']
        dataPDU     = EncodeAXDR.EncodeAXDR(' ',data)

        invokeID    = invokeIDAndPriority&0xff
        priority    = (invokeIDAndPriority&0xff00)>>8
        pdu         = '%04x%02x%02x%s'%(fullType,invokeID,priority,dataPDU)
    else:
        pdu         = ''

    return pdu


if __name__ == '__main__':

    xml = open(sys.argv[1]).read()
    d   = xmltodict.parse(xml)

    hex = EncodeDLMS(d)
    print(hex)

