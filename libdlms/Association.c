




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


void dlmsFormAARQ( Stream* stream )                      
{                                                                                                          
    OBISCode    object;

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


