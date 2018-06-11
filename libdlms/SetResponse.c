

#include "axdr.h"
#include "dlms.h"


//
// C501 81 00
//
// <SetResponse>
//   <SetResponseNormal>
//     <InvokeIdAndPriority Value="81" />
//     <Result Value="Success" />
//   </SetResponseNormal>
// </SetResponse>
//
void dlmsFormSetResponse( AXDRStream* stream, DataAccessResult result )
{
    axdrSetUint8( stream, set_response );   // type
    axdrSetUint8( stream, 0x01 );   // subType
    axdrSetUint8( stream, 0xc1 );   // invokeId
    
    axdrSetUint8( stream, result );   // invokeId
}

void dlmsParseSetResponse( AXDRStream* stream, DataAccessResult* resultType )
{
    uint8_t     type    = 0;
    uint8_t     subType = 0;
    uint8_t     invokeId= 0;
    uint8_t     result  = 0;

    axdrGetUint8( stream, &type );
    assert( type == set_response );

    axdrGetUint8( stream, &subType );
    assert( subType == 0x01 );

    axdrGetUint8( stream, &invokeId );

    axdrGetUint8( stream, &result );
    *resultType = (DataAccessResult)result;
}


