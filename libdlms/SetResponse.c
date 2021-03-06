

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
void dlmsFormSetResponse( Stream* stream, DataAccessResult result )
{
    streamSetUint8( stream, set_response );   // type
    streamSetUint8( stream, 0x01 );   // subType
    streamSetUint8( stream, 0xc1 );   // invokeId
    
    streamSetUint8( stream, result );   // invokeId
}

void dlmsParseSetResponse( Stream* stream, DataAccessResult* resultType )
{
    uint8_t     type    = 0;
    uint8_t     subType = 0;
    uint8_t     invokeId= 0;
    uint8_t     result  = 0;

    streamGetUint8( stream, &type );
    assert( type == set_response );

    streamGetUint8( stream, &subType );
    assert( subType == 0x01 );

    streamGetUint8( stream, &invokeId );

    streamGetUint8( stream, &result );
    *resultType = (DataAccessResult)result;
}


