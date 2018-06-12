

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
    streamSetUint8( stream, get_response );   // type
    streamSetUint8( stream, 0x01 );   // subType

    streamSetUint8( stream, 0x81 );   // invokeId

    streamSetUint8( stream, type );   // result type, 0=data, 1=returnCode.
}

void dlmsParseGetResponseNormal( AXDRStream* stream,  ResultType* resultType )
{
    uint8_t     type    = 0;
    uint8_t     subType = 0;
    uint8_t     invokeId= 0;
    uint8_t     result  = 0;

    streamGetUint8( stream, &type );
    assert( type == get_response );

    streamGetUint8( stream, &subType );
    assert( subType == 0x01 );

    streamGetUint8( stream, &invokeId );
    streamGetUint8( stream, &result );

    *resultType   = (DataAccessResult)result;
}


