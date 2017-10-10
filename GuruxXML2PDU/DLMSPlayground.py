


import DLMS
import xmltodict
import time
import serial
import sys
import binascii




sequenceNumber  = 1

def SequenceNumber():
    """
    """
    global sequenceNumber
    thisSequenceNumber  = sequenceNumber
    sequenceNumber      = (sequenceNumber + 1) & 0x7

    #print('*** New sequence number %d ***'%(thisSequenceNumber))

    return thisSequenceNumber


def ControlField():
    """
    """
    seq = SequenceNumber()
    control = (seq<<1) | (0x01<<4)

    return control


GetRequestTemplate = \
"""
<GetRequest>
  <GetRequestNormal>
    <InvokeIdAndPriority Value="81" />
    <AttributeDescriptor>
      <ClassId Value="%04x" />
      <InstanceId Value="%s" />
      <AttributeId Value="%x" />
    </AttributeDescriptor>
  </GetRequestNormal>
</GetRequest>
"""


def CreateGetRequest(ic, obis, attributeId):
    """
    DLMSPlayground.CreateGetRequest(8,'0203010000FF',2)
    """
    xml     = GetRequestTemplate%(ic,obis,attributeId) 
    d       = xmltodict.parse(xml)
    hdlc    = DLMS.DictToHDLC(d, controlField=ControlField() )

    return hdlc




GetRequestBlockTemplate = \
"""
<GetRequest>
 <GetRequestForNextDataBlock>
  <InvokeIdAndPriority Value="81" />
  <BlockNumber Value="%d" />
 </GetRequestForNextDataBlock>
</GetRequest>
"""


def CreateGetBlockRequest(blockNumber):
    """
    """
    xml     = GetRequestBlockTemplate%(blockNumber) 
    d       = xmltodict.parse(xml)
    hdlc    = DLMS.DictToHDLC(d, controlField=ControlField() )

    return hdlc




SetRequestTemplate_OctetString  = \
"""
<SetRequest>
  <SetRequestNormal>
    <InvokeIdAndPriority Value="81" />
    <AttributeDescriptor>
      <ClassId Value="%04d" />
      <InstanceId Value="%s" />
      <AttributeId Value="%d" />
    </AttributeDescriptor>
    <Value>
      <OctetString Value="%s" />
    </Value>
  </SetRequestNormal>
</SetRequest>
"""

def CreateSetRequest_OctetString(ic, obis, attributeId, value):
    """
    DLMSPlayground.CreateSetRequest_OctetString(8,'0203010000FF',2, "07D20C04030A060BFF007800")
    """
    xml     = SetRequestTemplate_OctetString%(ic,obis,attributeId, value) 
    d       = xmltodict.parse(xml)
    hdlc    = DLMS.DictToHDLC(d, controlField=0x32)

    return hdlc





AARQTemplate  = \
"""
<AssociationRequest>
  <ApplicationContextName Value="%s" />
  <SenderACSERequirements Value="1" />
  <MechanismName Value="%s" />
  <CallingAuthentication Value="%s" />
  <InitiateRequest>
    <ProposedDlmsVersionNumber Value="06" />
    <ProposedConformance>
      <ConformanceBit Name="Action" />
      <ConformanceBit Name="EventNotification" />
      <ConformanceBit Name="SelectiveAccess" />
      <ConformanceBit Name="Set" />
      <ConformanceBit Name="Get" />
      <ConformanceBit Name="InformationReport" />
      <ConformanceBit Name="MultipleReferences" />
      <ConformanceBit Name="BlockTransferWithAction" />
      <ConformanceBit Name="BlockTransferWithSetOrWrite" />
      <ConformanceBit Name="BlockTransferWithGetOrRead" />
    </ProposedConformance>
    <ProposedMaxPduSize Value="FFFF" />
  </InitiateRequest>
</AssociationRequest>
"""

def CreateAARQ(contextName, mechanismName, password):
    """
    DLMSPlayground.CreateAARQ('LN', 'Low', '3132333435363738')
    """
    xml     = AARQTemplate%(contextName, mechanismName, password) 
    d       = xmltodict.parse(xml)
    hdlc    = DLMS.DictToHDLC(d, controlField=0x10)

    return hdlc



def CreateSNRM():
    """
    DLMSPlayground.CreateSNRM()
    """
    #return '7ea023000200234193f736818014050200b4060200b407040000000108040000000115ca7e'
    return '7ea023000200234193f7368180140502010006020100070400000001080400000001696d7e'




def CreateDISC():
    """
    DLMSPlayground.CreateDISC()
    """
    return '7ea00a00020023415341d27e'







def OpenPortToMeter(portName):
    """
    port    = OpenPortToMeter('/dev/ttyUSB2')
    """
    port = serial.Serial(
        port=portName,
        baudrate=9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS
    )

    port.isOpen()

    return port


def SendHDLCToMeter(port, hdlcHex):
    """
    """
    port.write( binascii.unhexlify(hdlcHex) )


def GetResponseFromMeter(port):
    """
    """
    response    = ''
    while len(response) == 0:
        while port.inWaiting() > 0:
            response += port.read(1)

        response    = binascii.hexlify(response)
    print(response)
    return response





def TestMe():
    """
    """
    p=OpenPortToMeter('/dev/ttyUSB2')

    SendHDLCToMeter(p, CreateDISC() )
    time.sleep(1.0)
    print( GetResponseFromMeter(p) )

    SendHDLCToMeter(p, CreateSNRM() )
    time.sleep(1.0)
    print( GetResponseFromMeter(p) )

    SendHDLCToMeter(p, CreateAARQ('LN', 'Low', '3132333435363738') )
    time.sleep(1.0)
    print( DLMS.HDLCToDict(GetResponseFromMeter(p)) )

    rq    = CreateGetRequest(1,'0000600100FF',2)
    print(rq)
    SendHDLCToMeter(p, rq )
    time.sleep(1.0)
    rsp    = GetResponseFromMeter(p)
    print(rsp)
    print( DLMS.HDLCToDict(rsp) )
 


if __name__ == '__main__':
    TestMe()






