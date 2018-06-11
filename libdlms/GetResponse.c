

#include "axdr.h"
#include "dlms.h"



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


