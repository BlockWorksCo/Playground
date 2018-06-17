




#include "dlms.h"
#include "axdr.h"

const uint8_t   lnObjectId[] = {0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x01};
const uint8_t   snObjectId[] = {0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x02};

const uint8_t   llsMechanismName[]  = {0x60, 0x85, 0x74, 0x05, 0x08, 0x02, 0x01};
const uint8_t   hlsMechanismName[]  = {0x60, 0x85, 0x74, 0x05, 0x08, 0x02, 0x05};

const uint8_t   llsAuthenticationValue[]    = "12345678";
const uint8_t   hlsAuthenticationValue[]    = "K56iVagY";

const uint8_t   lnInitiateRequestAPDU[]     = {0x01, 0x00, 0x00, 0x00, 0x06, 0x5F, 0x1F, 0x04, 0x00, 0x00, 0x7E, 0x1F, 0x04, 0xB0};
const uint8_t   snInitiateRequestAPDU[]     = {0x01, 0x00, 0x00, 0x00, 0x06, 0x5F, 0x1F, 0x04, 0x00, 0x1c, 0x03, 0x20, 0x04, 0xB0};






void dlmsFormInitiateRequestAPDU( Stream* stream,  uint8_t dedicatedKey[],uint32_t dedicatedKeyLength, bool responseAllowed, int8_t proposedQualityOfService, uint8_t proposedDlmsVersion, uint32_t proposedConformanceFlags, uint16_t clientMaxReceivePDUSize )
{
    // InitiateRequest APDU tag
    streamSetUint8( stream, 0x01 );

    // dedicated-key
    if( dedicatedKeyLength > 0 )
    {
        streamSetUint8( stream, 0x01 ); // usage flag
        streamSetUint8( stream, dedicatedKeyLength ); // length
        streamSetUint8Array( stream, &dedicatedKey[0], dedicatedKeyLength ); // value.
    }
    else
    {
        streamSetUint8( stream, 0x00 ); // usage flag
    }

    // response-allowed
    if( responseAllowed > 0 )
    {
        streamSetUint8( stream, 0x01 ); // usage flag
    }
    else
    {
        streamSetUint8( stream, 0x00 ); // usage flag
    }

    // proposed-quality-of-service
    if( false )
    {
        streamSetUint8( stream, 0x01 ); // usage flag
        streamSetUint8( stream, proposedQualityOfService ); // value
    }
    else
    {
        streamSetUint8( stream, 0x00 ); // usage flag
    }

    // proposed-dlms-version-number
    if( true )
    {
        streamSetUint8( stream, proposedDlmsVersion ); // value
    }
    else
    {
        streamSetUint8( stream, 0x00 ); // usage flag
    }

    // proposed-conformance
    if( true )
    {
        streamSetUint8( stream, 0x5f ); // tag
        streamSetUint8( stream, 0x1f ); // tag
        streamSetUint8( stream, 0x04 ); // length
        streamSetUint8( stream, 0x00 ); // number of unused bits in last byte.
        streamSetUint8( stream, proposedConformanceFlags >> 16 ); // value
        streamSetUint8( stream, proposedConformanceFlags >> 8 ); // value
        streamSetUint8( stream, proposedConformanceFlags ); // value
    }
    else
    {
        streamSetUint8( stream, 0x00 ); // usage flag
    }

    // client-max-receive-pdu-size
    streamSetUint8( stream, clientMaxReceivePDUSize >> 8 ); // value
    streamSetUint8( stream, clientMaxReceivePDUSize ); // value
}


void dlmsFormAARQ( Stream* stream ) 
{                                                                                                          
    // PDU
    streamSetUint8( stream, aarq ); // tag
    streamSetUint8( stream, 0x1D ); // length (no security).

    // Application context name
    streamSetUint8( stream, 0xa1 ); // tag
    streamSetUint8( stream, 0x09 ); // length.

    // Object identifier.
    streamSetUint8( stream, 0x06 ); // tag
    streamSetUint8( stream, sizeof(lnObjectId) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&lnObjectId[0], sizeof(lnObjectId) ); // value.
                                                                                                           
    // requirements field
    streamSetUint8( stream, 0x8a ); // tag
    streamSetUint8( stream, 0x02 ); // length.
    streamSetUint8( stream, 0x07 ); // value (number of unused bits in the following bit stream, i.e 1 bit used).
    streamSetUint8( stream, 0x80 ); // value (authentication functional unit).

    // mechanism name
    streamSetUint8( stream, 0x8b ); // tag
    streamSetUint8( stream, sizeof(llsMechanismName) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&llsMechanismName[0], sizeof(llsMechanismName) ); // value.

    // calling authenication field
    streamSetUint8( stream, 0xac ); // tag
    streamSetUint8( stream, 0x0a ); // length.
}                                                                                                          



void dlmsFormAARQForLLS( Stream* stream )                      
{                                                                                                          
    OBISCode    object;

    // PDU
    streamSetUint8( stream, aarq ); // tag
    streamSetUint8( stream, 0x36 ); // length (ll security).

    // Application context name
    streamSetUint8( stream, 0xa1 ); // tag
    streamSetUint8( stream, 0x09 ); // length.

    // Object identifier.
    streamSetUint8( stream, 0x06 ); // tag
    streamSetUint8( stream, sizeof(lnObjectId) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&lnObjectId[0], sizeof(lnObjectId) ); // value.
                                                                                                           
    // requirements field
    streamSetUint8( stream, 0x8a ); // tag
    streamSetUint8( stream, 0x02 ); // length.
    streamSetUint8( stream, 0x07 ); // value (number of unused bits in the following bit stream, i.e 1 bit used).
    streamSetUint8( stream, 0x80 ); // value (authentication functional unit).

    // mechanism name
    streamSetUint8( stream, 0x8b ); // tag
    streamSetUint8( stream, sizeof(llsMechanismName) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&llsMechanismName[0], sizeof(llsMechanismName) ); // value.

    // calling authenication field
    streamSetUint8( stream, 0xac ); // tag
    streamSetUint8( stream, 0x0a ); // length.

    // calling authenication value
    streamSetUint8( stream, 0x80 ); // tag
    streamSetUint8( stream, sizeof(llsAuthenticationValue) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&llsAuthenticationValue[0], sizeof(llsAuthenticationValue) ); // value.

    // user information
    streamSetUint8( stream, 0xbe ); // tag
    streamSetUint8( stream, 0x10 ); // length.

    // user information: initiate-request APDU
    streamSetUint8( stream, 0x04 ); // type
    streamSetUint8( stream, sizeof(lnInitiateRequestAPDU) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&lnInitiateRequestAPDU[0], sizeof(lnInitiateRequestAPDU) ); // value.

}                                                                                                          



void dlmsFormAARQForHLS( Stream* stream )                      
{                                                                                                          
    OBISCode    object;

    // PDU
    streamSetUint8( stream, aarq ); // tag
    streamSetUint8( stream, 0x36 ); // length (ll security).

    // Application context name
    streamSetUint8( stream, 0xa1 ); // tag
    streamSetUint8( stream, 0x09 ); // length.

    // Object identifier.
    streamSetUint8( stream, 0x06 ); // tag
    streamSetUint8( stream, sizeof(lnObjectId) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&lnObjectId[0], sizeof(lnObjectId) ); // value.
                                                                                                           
    // requirements field
    streamSetUint8( stream, 0x8a ); // tag
    streamSetUint8( stream, 0x02 ); // length.
    streamSetUint8( stream, 0x07 ); // value (number of unused bits in the following bit stream, i.e 1 bit used).
    streamSetUint8( stream, 0x80 ); // value (authentication functional unit).

    // mechanism name
    streamSetUint8( stream, 0x8b ); // tag
    streamSetUint8( stream, sizeof(hlsMechanismName) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&hlsMechanismName[0], sizeof(hlsMechanismName) ); // value.

    // calling authenication field
    streamSetUint8( stream, 0xac ); // tag
    streamSetUint8( stream, 0x0a ); // length.

    // calling authenication value
    streamSetUint8( stream, 0x80 ); // tag
    streamSetUint8( stream, sizeof(hlsAuthenticationValue) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&hlsAuthenticationValue[0], sizeof(hlsAuthenticationValue) ); // value.

    // user information
    streamSetUint8( stream, 0xbe ); // tag
    streamSetUint8( stream, 0x10 ); // length.

    // user information: initiate-request APDU
    streamSetUint8( stream, 0x04 ); // type
    streamSetUint8( stream, sizeof(lnInitiateRequestAPDU) ); // length.
    streamSetUint8Array( stream, (uint8_t*)&lnInitiateRequestAPDU[0], sizeof(lnInitiateRequestAPDU) ); // value.

}                                                                                                          



void dlmsParseAARQ( Stream* stream, char* mechanismName, uint32_t mechanismNameMaxSize )
{
    // PDU
    uint8_t     tag;
    streamGetUint8( stream, &tag ); // tag
    assert( tag == aarq );

    uint8_t     length;
    streamGetUint8( stream, &length ); // length (no security).
    //assert( length == 0x1d );

    // Application context name
    streamGetUint8( stream, &tag ); // tag
    assert( tag == 0xa1 );
    streamGetUint8( stream, &length ); // length.
    //assert( length == 9 );

    // Object identifier.
    streamGetUint8( stream, &tag ); // tag
    assert( tag == 0x06 );
    streamGetUint8( stream, &length ); // length.
    uint8_t     objectId[7]     = {0};
    streamGetUint8Array( stream, (uint8_t*)&objectId[0], sizeof(objectId) ); // value.
                                                                                                           
    // requirements field
    streamGetUint8( stream, &tag ); // tag
    assert( tag == 0x8a );
    streamGetUint8( stream, &length ); // length.
    assert( length == 0x02 );
    uint8_t numberOfUnusedBitsInByte    = 0;
    streamGetUint8( stream, &numberOfUnusedBitsInByte ); // value (number of unused bits in the following bit stream, i.e 1 bit used).
    uint8_t requirements    = 0;
    streamGetUint8( stream, &requirements ); // value (authentication functional unit).

    // mechanism name
    streamGetUint8( stream, &tag ); // tag
    assert( tag == 0x8b );
    streamGetUint8( stream, &length ); // length.
    assert( length == sizeof(llsMechanismName) );
    assert( length <= mechanismNameMaxSize );
    streamGetUint8Array( stream, (uint8_t*)&mechanismName[0], length ); // value.

    // calling authenication field
    streamGetUint8( stream, &tag ); // tag
    assert( tag == 0xac );
    streamGetUint8( stream, &length ); // length.
    assert( length == 10 );
}




