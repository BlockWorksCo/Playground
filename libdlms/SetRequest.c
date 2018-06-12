

#include "axdr.h"
#include "dlms.h"


//
// C101 C1 0004 0000600200FF 04 00 090870726F6720696420
//
// <SetRequest>
//   <SetRequestNormal>
//     <InvokeIdAndPriority Value="C1" />
//     <AttributeDescriptor>
//       <!--EXTENDED_REGISTER-->
//       <ClassId Value="0004" />
//       <!--0.0.96.2.0.255-->
//       <InstanceId Value="0000600200FF" />
//       <AttributeId Value="04" />
//     </AttributeDescriptor>
//     <Value>
//       <!--prog id -->
//       <OctetString Value="70726F6720696420" />
//     </Value>
//   </SetRequestNormal>
// </SetRequest>
//
void dlmsFormSetRequest( AXDRStream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId )
{
    streamSetUint8( stream, set_request );   // type
    streamSetUint8( stream, 0x01 );   // subType
    streamSetUint8( stream, 0xc1 );   // invokeId
    
    dlmsFormAttributeDescriptor( stream, ifClass, obisCode, attributeId );
}

void dlmsParseSetRequest( AXDRStream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId )
{
    uint8_t     type    = 0;
    uint8_t     subType = 0;
    uint8_t     invokeId= 0;
    uint8_t     result  = 0;
    uint16_t    ic      = 0;
    uint8_t     sa      = 0;
    uint8_t     attr    = 0;

    streamGetUint8( stream, &type );
    assert( type == set_request );

    streamGetUint8( stream, &subType );
    assert( subType == 0x01 );

    streamGetUint8( stream, &invokeId );

    dlmsParseAttributeDescriptor( stream, ifClass, obisCode, attrId );
}


