



import DLMSPlayground
import DLMS
import time
import xmltodict
import re



OBISList    = \
[
   {'code':[0x00,0x00,0x60,0x0a,0x02,0xff], 'ic':5 },   # 
   {'code':[0x01,0x00,0x0d,0x1b,0x00,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x20,0x1b,0x00,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x34,0x1b,0x00,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x48,0x1b,0x00,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x1f,0x1b,0x00,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x33,0x1b,0x00,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x47,0x1b,0x00,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x20,0x18,0x7c,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x34,0x18,0x7c,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x48,0x18,0x7c,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x1f,0x18,0x7c,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x33,0x18,0x7c,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x47,0x18,0x7c,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x21,0x1b,0x00,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x35,0x1b,0x00,0xff], 'ic':5 },
   {'code':[0x01,0x00,0x49,0x1b,0x00,0xff], 'ic':5 },

   {'code':[ 0,0,1,0,0,255 ], 'ic':8 },
   {'code':[ 1,0,0,1,2,255 ], 'ic':1 }, # 
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
    time.sleep(0.5)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d= DLMS.HDLCToDict(rsp)
    print(d)

    ISCaptureObjects    = d['GetResponse']['GetResponsewithDataBlock']['Result']['Result']['RawData']['@Value']

    for i in range(1,100):

        rq  = DLMSPlayground.CreateGetRequestforNextDataBlock(i)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(0.5)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        if len(rsp) == 0:
            break
        else:
            ISCaptureObjects   = ISCaptureObjects + d['GetResponse']['GetResponsewithDataBlock']['Result']['Result']['RawData']['@Value']

        print(rsp)
        print( DLMS.HDLCToDict(rsp) )

    print('IS capture objects: \n\n[%s]\n\n'%ISCaptureObjects)

    print('----- now reading buffer -----');


    #
    # read buffer (values)
    #
    rq    = DLMSPlayground.CreateGetRequest(7,hexCode,2)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(0.5)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d   = DLMS.HDLCToDict(rsp)
    print(d)

    ISProfileBuffer   = d['GetResponse']['GetResponsewithDataBlock']['Result']['Result']['RawData']['@Value']

    for i in range(1,20):

        rq  = DLMSPlayground.CreateGetRequestforNextDataBlock(i)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(0.5)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        if len(rsp) == 0:
            break

        xml = DLMS.HDLCToXML(rsp)
        print(xml)
        if xml[0:6] == '<Data=': 
            xml = xml.replace('=',' Value=')    # xml is not valid, no tag, just attribute, fix it up.
            d = xmltodict.parse(xml)
            dataField   = d['Data']['@Value']
        else:
            d = xmltodict.parse(xml)
            dataField   = d['GetResponse']['GetResponsewithDataBlock']['Result']['Result']['RawData']['@Value']

        ISProfileBuffer = ISProfileBuffer + dataField

        print(rsp)
        print(d)


    print('IS Profile buffer: %s'%ISProfileBuffer)




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
        ic  = int(obisInfo['ic'])

        hexCode = '%02x%02x%02x%02x%02x%02x'%(a,b,c,d,e,f)
        print('------- getting %s ic=%d --------'%(hexCode,ic))

        if ic == 3:
            rq    = DLMSPlayground.CreateGetRequest(ic,hexCode,3)
        elif ic == 4:
            rq    = DLMSPlayground.CreateGetRequest(ic,hexCode,4)
        elif ic == 5:
            rq    = DLMSPlayground.CreateGetRequest(ic,hexCode,4)
        elif ic == 8:
            rq    = DLMSPlayground.CreateGetRequest(ic,hexCode,3)
        elif ic == 1:
            rq    = DLMSPlayground.CreateGetRequest(ic,hexCode,3)
        else:
            print('oops...')
            pass
            
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

            
     

def ReadLoadProfile():
    """
    """

    p   = Associate()

    #hexCode = '%02x%02x%02x%02x%02x%02x'%(1,0,99,1,1,255)
    hexCode = '%02x%02x%02x%02x%02x%02x'%(1,0,98,1,0,255)

    #
    # read capture_period.
    #
    print('------- getting %s --------'%hexCode)

    rq    = DLMSPlayground.CreateGetRequest(7,hexCode,4)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(0.5)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d= DLMS.HDLCToDict(rsp)
    print(d)

    capturePeriod   = d['GetResponse']['GetResponseNormal']['Result']['Data']['UInt32']['@Value']
    print('Capture period is %s '%capturePeriod)

    #
    # read entries_in_use.
    #
    print('------- getting %s --------'%hexCode)

    rq    = DLMSPlayground.CreateGetRequest(7,hexCode,7)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(0.5)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d= DLMS.HDLCToDict(rsp)
    print(d)

    entriesInUse   = d['GetResponse']['GetResponseNormal']['Result']['Data']['UInt32']['@Value']
    print('Entries in useis %s '%entriesInUse)

    #
    # read profile_entries
    #
    print('------- getting %s --------'%hexCode)

    rq    = DLMSPlayground.CreateGetRequest(7,hexCode,8)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(0.5)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d= DLMS.HDLCToDict(rsp)
    print(d)

    profileEntries   = d['GetResponse']['GetResponseNormal']['Result']['Data']['UInt32']['@Value']
    print('profile_entries = %s '%entriesInUse)

    #
    # read sort_method
    #
    print('------- getting %s --------'%hexCode)

    rq    = DLMSPlayground.CreateGetRequest(7,hexCode,5)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(0.5)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d= DLMS.HDLCToDict(rsp)
    print(d)

    sortMethod   = d['GetResponse']['GetResponseNormal']['Result']['Data']['Enum']['@Value']
    print('sort_method = %s '%sortMethod)

    #
    # Call the capture method.
    #
    #print('------- Calling capture_objects --------')
    #rq  = DLMSPlayground.CreateActionRequest_NoParams(7, hexCode, 2 ) 
    #print(rq)
    #DLMSPlayground.SendHDLCToMeter(p, rq )
    #time.sleep(0.5)
    #rsp    = DLMSPlayground.GetResponseFromMeter(p)
    #print(rsp)
    #d= DLMS.HDLCToDict(rsp)
    #print(d)

    #
    # read capture_objects.
    #
    template    = \
    """
    <GetRequest>
      <GetRequestNormal>
        <InvokeIdAndPriority Value="129" />
        <AttributeDescriptor>
          <!--PROFILE_GENERIC-->
          <ClassId Value="7" />
          <!--1.0.99.1.1.255-->
          <InstanceId Value="0100620100FF" />
          <AttributeId Value="3" />
        </AttributeDescriptor>
      </GetRequestNormal>
    </GetRequest>

    """
    xml     = template 
    d       = xmltodict.parse(xml)
    rq      = DLMS.DictToHDLC(d, controlField=DLMSPlayground.ControlField())


    lastBlock   = 0

    print('\n\nReading 0')
    #rq    = DLMSPlayground.CreateGetRequest(7,hexCode,3)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(1.0)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d= DLMS.HDLCToDict(rsp)
    print(d)

    try:
        LSCaptureObjects    = d['GetResponse']['GetResponsewithDataBlock']['Result']['Result']['RawData']['@Value']
        print('LSCaptureObjects = [%s]'%LSCaptureObjects)
    except KeyError:
        qty    = int(d['GetResponse']['GetResponseNormal']['Result']['Data']['Array']['@Qty'])
        if qty == 0:
            print('----> qty is zero <-----')
            lastBlock   = 255
            LSCaptureObjects    = ''
        print('LSCaptureObjects = [%s]'%LSCaptureObjects)
        

    i   = 1
    while lastBlock == 0:


        print('\n\nReading capture_objects %d'%i)
        rq  = DLMSPlayground.CreateGetRequestforNextDataBlock(i)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(1.0)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        if len(rsp) == 0:
            print('----> No response, breaking out.')
            break
        else:
            print('rsp = %s'%rsp)
            d= DLMS.HDLCToDict(rsp)
            print(rsp)
            print(d)
            lastBlock   = int(d['GetResponse']['GetResponsewithDataBlock']['Result']['LastBlock']['@Value'], 16)
            print('------> lastBlock = [%d]'%lastBlock)
            thisBlock   = d['GetResponse']['GetResponsewithDataBlock']['Result']['Result']['RawData']['@Value']  
            LSCaptureObjects   = LSCaptureObjects + thisBlock
            print('LSCaptureObjects = [%s]'%thisBlock)

        i   += 1


    print('LS capture objects: \n\n[%s]\n\n'%LSCaptureObjects)
    open('LSData.hex','w').write(LSCaptureObjects)





def ActionTest():
    """
    """
    p   = Associate()

    hexCode = '%02x%02x%02x%02x%02x%02x'%(1,0,99,1,1,255)

    #
    # Call the capture method.
    #
    rq  = DLMSPlayground.CreateActionRequest_NoParams(7, hexCode, 4 ) 
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(0.5)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d= DLMS.HDLCToDict(rsp)
    print(d)



def GetTest():
    """
    """
    p   = Associate()

    hexCode = '%02x%02x%02x%02x%02x%02x'%(1,0,99,1,1,255)

    for i in range(4,8):
        print('\n\nDoing Get')
        rq    = DLMSPlayground.CreateGetRequest(7,hexCode,i)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(0.5)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        print(rsp)
        d= DLMS.HDLCToDict(rsp)
        print(d)



def TimeGet():
    """
    """
    template    = \
    """
    <GetRequest>
      <GetRequestNormal>
        <InvokeIdAndPriority Value="129" />
        <AttributeDescriptor>
          <!--CLOCK-->
          <ClassId Value="8" />
          <!--0.0.1.0.0.255-->
          <InstanceId Value="0000010000FF" />
          <AttributeId Value="2" />
        </AttributeDescriptor>
      </GetRequestNormal>
    </GetRequest>
    """
    p   = Associate()

    hexCode = '0000010000FF'
    rq    = DLMSPlayground.CreateGetRequest(8,hexCode,2)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(0.5)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d= DLMS.HDLCToDict(rsp)
    print(d)

    timeHex = d['GetResponse']['GetResponseNormal']['Result']['Data']['OctetString']['@Value']
    print('time is: %s'%(timeHex))




def TimeGet():
    """
    """
    template    = \
    """
    <GetRequest>
      <GetRequestNormal>
        <InvokeIdAndPriority Value="129" />
        <AttributeDescriptor>
          <!--CLOCK-->
          <ClassId Value="8" />
          <!--0.0.1.0.0.255-->
          <InstanceId Value="0000010000FF" />
          <AttributeId Value="2" />
        </AttributeDescriptor>
      </GetRequestNormal>
    </GetRequest>
    """
    p   = Associate()

    hexCode = '0000010000FF'
    rq    = DLMSPlayground.CreateGetRequest(8,hexCode,2)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(0.5)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    d= DLMS.HDLCToDict(rsp)
    print(d)

    timeHex = d['GetResponse']['GetResponseNormal']['Result']['Data']['OctetString']['@Value']
    print('time is: %s'%(timeHex))





if __name__ == '__main__':
    #ReadInstantaneousProfile()
    #StaticDiscovery()
    ReadLoadProfile()
    #ReadObjectList()

    #ActionTest()
    #GetTest()
    #TimeGet()

