



import DLMSPlayground
import DLMS
import time
import xmltodict
import re



OBISList    = \
[
   {'code':[ 0,0,1,0,0,255 ], 'ic':8 },
   {'code':[ 1,0,0,1,2,255 ], 'ic':1 },
   {'code':[ 1,0,31,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,51,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,71,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,32,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,52,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,72,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,81,7,4,255 ], 'ic':3 },
   {'code':[ 1,0,81,7,15,255 ], 'ic':3 },
   {'code':[ 1,0,81,7,26,255 ], 'ic':3 },
   {'code':[ 1,0,13,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,33,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,53,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,73,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,1,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,2,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,21,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,22,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,41,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,42,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,61,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,62,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,3,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,4,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,23,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,24,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,43,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,44,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,63,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,64,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,1,6,0,255 ], 'ic':4 },
   {'code':[ 1,0,1,6,1,255 ], 'ic':4 },
   {'code':[ 1,0,1,6,2,255 ], 'ic':4 },
   {'code':[ 1,0,1,6,3,255 ], 'ic':4 },
   {'code':[ 1,0,1,6,4,255 ], 'ic':4 },
   {'code':[ 1,0,2,6,0,255 ], 'ic':4 },
   {'code':[ 1,0,2,6,1,255 ], 'ic':4 },
   {'code':[ 1,0,2,6,2,255 ], 'ic':4 },
   {'code':[ 1,0,2,6,3,255 ], 'ic':4 },
   {'code':[ 1,0,2,6,4,255 ], 'ic':4 },
   {'code':[ 1,0,1,8,0,255 ], 'ic':3 },
   {'code':[ 1,0,1,8,1,255 ], 'ic':3 },
   {'code':[ 1,0,1,8,2,255 ], 'ic':3 },
   {'code':[ 1,0,1,8,3,255 ], 'ic':3 },
   {'code':[ 1,0,1,8,4,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,0,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,1,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,2,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,3,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,4,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,0,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,1,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,2,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,3,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,4,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,0,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,1,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,2,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,3,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,4,255 ], 'ic':3 },
   {'code':[ 1,0,3,6,0,255 ], 'ic':4 },
   {'code':[ 1,0,3,6,1,255 ], 'ic':4 },
   {'code':[ 1,0,3,6,2,255 ], 'ic':4 },
   {'code':[ 1,0,3,6,3,255 ], 'ic':4 },
   {'code':[ 1,0,3,6,4,255 ], 'ic':4 },
   {'code':[ 1,0,4,6,0,255 ], 'ic':4 },
   {'code':[ 1,0,4,6,1,255 ], 'ic':4 },
   {'code':[ 1,0,4,6,2,255 ], 'ic':4 },
   {'code':[ 1,0,4,6,3,255 ], 'ic':4 },
   {'code':[ 1,0,4,6,4,255 ], 'ic':4 },
   {'code':[ 1,0,3,8,0,255 ], 'ic':3 },
   {'code':[ 1,0,3,8,1,255 ], 'ic':3 },
   {'code':[ 1,0,3,8,2,255 ], 'ic':3 },
   {'code':[ 1,0,3,8,3,255 ], 'ic':3 },
   {'code':[ 1,0,3,8,4,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,0,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,1,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,2,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,3,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,4,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,0,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,1,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,2,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,3,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,4,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,0,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,1,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,2,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,3,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,3,255 ], 'ic':3 },
]



def Associate():
    """
    """
    p=DLMSPlayground.OpenPortToMeter('/dev/ttyUSB2')
    
    DLMSPlayground.SendHDLCToMeter(p, DLMSPlayground.CreateDISC() )
    time.sleep(0.5)
    print( DLMSPlayground.GetResponseFromMeter(p) )

    DLMSPlayground.SendHDLCToMeter(p, DLMSPlayground.CreateSNRM() )
    time.sleep(0.5)
    print( DLMSPlayground.GetResponseFromMeter(p) )

    DLMSPlayground.SendHDLCToMeter(p, DLMSPlayground.CreateAARQ('LN', 'Low', '3132333435363738') )
    time.sleep(0.5)
    print( DLMS.HDLCToDict(DLMSPlayground.GetResponseFromMeter(p)) )

    return p



def ReadInstantaneousProfile():
    """
    """

    p   = Associate()

    print('----- now capture_objects -----');

    #
    # read capture_objects.
    #
    hexCode = '%02x%02x%02x%02x%02x%02x'%(1,0,94,66,0,255)
    print('------- getting %s --------'%hexCode)

    rq    = DLMSPlayground.CreateGetRequest(7,hexCode,3)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(1.0)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    print( DLMS.HDLCToDict(rsp) )

    for i in range(1,5):

        rq  = DLMSPlayground.CreateGetRequestforNextDataBlock(i)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(1.0)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        print(rsp)
        print( DLMS.HDLCToDict(rsp) )


    print('----- now reading buffer -----');


    #
    # read buffer (values)
    #
    hexCode = '%02x%02x%02x%02x%02x%02x'%(1,0,94,66,0,255)
    print('------- getting %s --------'%hexCode)

    rq    = DLMSPlayground.CreateGetRequest(7,hexCode,2)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(1.0)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    print( DLMS.HDLCToDict(rsp) )

    for i in range(1,5):

        rq  = DLMSPlayground.CreateGetRequestforNextDataBlock(i)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(1.0)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        print(rsp)
        print( DLMS.HDLCToDict(rsp) )





def ReadObjectList():
    """
    C0 01 81 00 0F 00 00 28 00 00 FF 02 00
    """
   

    p   = Associate()

    objectList  = ''

    hexCode = '%02x%02x%02x%02x%02x%02x'%(0,0,40,0,0,255)
    print('------- getting %s --------'%hexCode)

    rq    = DLMSPlayground.CreateGetRequest(15,hexCode,2)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(1.0)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d   = DLMS.HDLCToDict(rsp)
    print(d)


    objectList  = objectList + d['GetResponse']['GetResponsewithDataBlock']['Result']['Result']['RawData']['@Value']

    for i in range(1,200):

        print('--------> Block %d <-------'%i)
    
        rq  = DLMSPlayground.CreateGetRequestforNextDataBlock(i)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(0.5)

        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        if len(rsp) > 0:
            print('block response: [%s]'%rsp)
            xml = DLMS.HDLCToXML(rsp)
            #xml = xml.replace('=',' Value=')    # xml is not valid, no tag, just attribute, fix it up.
            print('response = %s'%xml)
            d   = xmltodict.parse(xml)
            print(d)
            print(rsp)
            #print( DLMS.HDLCToDict(rsp) )
        
            objectList  = objectList + d['GetResponse']['GetResponsewithDataBlock']['Result']['Result']['RawData']['@Value']
        else:
            break


    print(objectList)




def StaticDiscovery():
    """
    """
    p   = Associate()

    for obisInfo in OBISList:
        a   = obisInfo['code'][0]
        b   = obisInfo['code'][1]
        c   = obisInfo['code'][2]
        d   = obisInfo['code'][3]
        e   = obisInfo['code'][4]
        f   = obisInfo['code'][5]
        ic  = obisInfo['ic']

        hexCode = '%02x%02x%02x%02x%02x%02x'%(a,b,c,d,e,f)
        print('------- getting %s --------'%hexCode)

        rq    = DLMSPlayground.CreateGetRequest(ic,hexCode,3)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(0.5)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        print(rsp)
        print( DLMS.HDLCToDict(rsp) )
     
        if rsp != None:
            xmlAttr3     = xmltodict.unparse(DLMS.HDLCToDict(rsp))


        rq    = DLMSPlayground.CreateGetRequest(ic,hexCode,2)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(0.5)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        print(rsp)
        print( DLMS.HDLCToDict(rsp) )
     
        if rsp != None:
            xmlAttr2     = xmltodict.unparse(DLMS.HDLCToDict(rsp))


        combinedXML = '<Object><OBIS value="%s" /><ClassID Value="%d" /><Attribute2Read>%s</Attribute2Read>\n<Attribute3Read>%s</Attribute3Read></Object>'%(hexCode,ic,xmlAttr2,xmlAttr3)
        combinedXML = re.sub(r'<\?.*\?>','',combinedXML)
        open('Objects/%s.xml'%(hexCode),'w+').write(combinedXML)

            
     



if __name__ == '__main__':
    #ReadInstantaneousProfile()
    StaticDiscovery()
    #ReadObjectList()

