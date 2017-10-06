


import DLMS
import xmltodict
import time
import serial
import sys
import binascii






GetRequestTemplate = \
"""
<GetRequest>
  <GetRequestNormal>
    <InvokeIdAndPriority Value="81" />
    <AttributeDescriptor>
      <ClassId Value="%04d" />
      <InstanceId Value="%s" />
      <AttributeId Value="%d" />
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
    hdlc    = DLMS.DictToHDLC(d)

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
    hdlc    = DLMS.DictToHDLC(d)

    return hdlc





AARETemplate  = \
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

def CreateAARE(contextName, mechanismName, password):
    """
    DLMSPlayground.CreateAARE('LN', 'Low', '3132333435363738')
    """
    xml     = AARETemplate%(contextName, mechanismName, password) 
    d       = xmltodict.parse(xml)
    hdlc    = DLMS.DictToHDLC(d)

    return hdlc



def CreateSNRM():
    """
    DLMSPlayground.CreateSNRM()
    """
    #return '7ea023000200234193f736818014050200b4060200b407040000000108040000000115ca7e'
    return '7ea00a410002002373fd607e'




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
    while port.inWaiting() > 0:
        response += port.read(1)

    response    = binascii.hexlify(response)
    #print(response)
    return response





def TestMe():
    """
    """
    p=OpenPortToMeter('/dev/ttyUSB2')
    snrm=CreateSNRM()
    SendHDLCToMeter(p, snrm)
    time.sleep(1.0)
    GetResponseFromMeter(p)
 

