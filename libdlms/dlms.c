

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
void dlmsFormGetRequest( AXDRStream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId, uint8_t accessSelector )
{
    axdrSetUint8( stream, get_request );   // type
    axdrSetUint8( stream, 0x01 );   // subType
    axdrSetUint8( stream, 0x81 );   // invokeId
    
    dlmsFormAttributeDescriptor( stream, ifClass, obisCode, attributeId );

    axdrSetUint8( stream, accessSelector );   // access selector
}

void dlmsParseGetRequest( AXDRStream* stream,  OBISCode* obisCode, InterfaceClass* ifClass, AttributeId* attributeId, uint8_t* accessSelector )
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

    axdrGetUint8( stream, accessSelector );
}


//
// C401 09 00 09 06 456C73746572
//
//   <GetResponseNormal>
//    <InvokeIdAndPriority Value="129" />
//    <Result>
//      <Data>
//        <!--Elster-->
//        <OctetString Value="456C73746572" />
//      </Data>
//    </Result>
//  </GetResponseNormal>
//
void dlmsFormGetResponseNormal( AXDRStream* stream,  ResultType type )
{
    axdrSetUint8( stream, get_response );   // type
    axdrSetUint8( stream, 0x01 );   // subType

    axdrSetUint8( stream, 0x81 );   // invokeId

    axdrSetUint8( stream, type );   // result type, 0=data, 1=returnCode.
}

void dlmsParseGetResponseNormal( AXDRStream* stream,  ResultType* resultType )
{
    uint8_t     type    = 0;
    uint8_t     subType = 0;
    uint8_t     invokeId= 0;
    uint8_t     result  = 0;

    axdrGetUint8( stream, &type );
    assert( type == get_response );

    axdrGetUint8( stream, &subType );
    assert( subType == 0x01 );

    axdrGetUint8( stream, &invokeId );
    axdrGetUint8( stream, &result );

    *resultType   = (DataAccessResult)result;
}


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
void dlmsFormSetRequest( AXDRStream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId, uint8_t accessSelector )
{
    axdrSetUint8( stream, set_request );   // type
    axdrSetUint8( stream, 0x01 );   // subType
    axdrSetUint8( stream, 0xc1 );   // invokeId
    
    dlmsFormAttributeDescriptor( stream, ifClass, obisCode, attributeId );

    axdrSetUint8( stream, accessSelector );   // message-specific access selector.
}

void dlmsParseSetRequest( AXDRStream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId, uint8_t* accessSelector )
{
    uint8_t     type    = 0;
    uint8_t     subType = 0;
    uint8_t     invokeId= 0;
    uint8_t     result  = 0;
    uint16_t    ic      = 0;
    uint8_t     sa      = 0;
    uint8_t     attr    = 0;

    axdrGetUint8( stream, &type );
    assert( type == set_request );

    axdrGetUint8( stream, &subType );
    assert( subType == 0x01 );

    axdrGetUint8( stream, &invokeId );

    dlmsParseAttributeDescriptor( stream, ifClass, obisCode, attrId );

    axdrGetUint8( stream, accessSelector );
}


//
// c001810007 0100620100ff 02 
//   full/NoSA --> 00
//   by entry   --> 01 02 0204 0600000005 0600000005 120001 120013 
//   by range  -->
//
// By range:
// <AccessSelection>
//      <AccessSelector Value="1" />
//      <AccessParameters>
//        <Structure Qty="4" >
//          <Structure Qty="4" >
//            <UInt16 Value="8" />
//            <!--0.0.1.0.0.255-->
//            <OctetString Value="0000010000FF" />
//            <Int8 Value="2" />
//            <UInt16 Value="0" />
//          </Structure>
//          <!--2018-06-07 17:16:27-->
//          <OctetString Value="07E206070411101BFFFFC480" />
//          <!--2018-06-07 17:30:27-->
//          <OctetString Value="07E2060704111E1BFFFFC480" />
//          <Array Qty="0" >
//          </Array>
//        </Structure>
//      </AccessParameters>
//    </AccessSelection>
//
// By entry:
//    <AccessSelection>
//      <AccessSelector Value="02" />
//      <AccessParameters>
//        <Structure Qty="04" >
//          <UInt32 Value="00000005" />
//          <UInt32 Value="00000005" />
//          <UInt16 Value="0001" />
//          <UInt16 Value="0013" />
//        </Structure>
//      </AccessParameters>
//    </AccessSelection>
//
void dlmsParseAccessSelection( AXDRStream* stream, uint8_t* accessSelector )
{
    axdrGetUint8( stream, accessSelector );
}



//
//
//
void dlmsFormAttributeDescriptor( AXDRStream* stream, InterfaceClass ifClass, OBISCode obisCode, AttributeId attrId )
{
    uint16_t    ic  = ifClass;
    axdrSetUint8Array( stream, (void*)&ic, sizeof(ic) );

    axdrSetUint8Array( stream, obisCode,sizeof(OBISCode) );
    
    axdrSetUint8( stream, attrId );
}

void dlmsParseAttributeDescriptor( AXDRStream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId )
{
    uint16_t ic;
    axdrGetUint8Array( stream, (void*)&ic,sizeof(ic) );
    *ifClass    = (InterfaceClass)ic;

    axdrGetUint8Array( stream, (void*)obisCode,sizeof(OBISCode) );

    uint8_t attr;
    axdrGetUint8( stream, &attr );
    *attrId  = (AttributeId)attr;
}

