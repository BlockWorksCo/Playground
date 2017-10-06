


import DLMS
import xmltodict





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



