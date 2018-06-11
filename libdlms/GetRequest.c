

#include "axdr.h"
#include "dlms.h"



//
// C001 81 0008 0000010000FF 02 00
//
// <GetRequest>
// <GetRequestNormal>
// <InvokeIdAndPriority Value="81" />
// <AttributeDescriptor>
// <!--CLOCK-->
// <ClassId Value="0008" />
// <!--0.0.1.0.0.255-->
// <InstanceId Value="0000010000FF" />
// <AttributeId Value="02" />
// </AttributeDescriptor>
// </GetRequestNormal>
// </GetRequest>
//
void dlmsFormGetRequest( AXDRStream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId )
{
    axdrSetUint8( stream, get_request );   // type
    axdrSetUint8( stream, 0x01 );   // subType
    axdrSetUint8( stream, 0x81 );   // invokeId
    
    dlmsFormAttributeDescriptor( stream, ifClass, obisCode, attributeId );
}

void dlmsParseGetRequest( AXDRStream* stream,  OBISCode* obisCode, InterfaceClass* ifClass, AttributeId* attributeId )
{
    uint8_t     type    = 0;
    uint8_t     subType = 0;
    uint8_t     invokeId= 0;
    uint16_t    ic      = 0;
    uint16_t    attr    = 0;

    axdrGetUint8( stream, &type );
    assert( type == get_request );

    axdrGetUint8( stream, &subType );
    assert( subType == 0x01 );

    axdrGetUint8( stream, &invokeId );

    dlmsParseAttributeDescriptor( stream, ifClass, obisCode, attributeId );
}


