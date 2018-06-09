


#include "axdr.h"
#include "dlms.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


void printHex(uint8_t value)
{
    printf("%02X",value);
}

void printHexData(uint8_t* data, uint32_t numberOfBytes)
{
    for(uint32_t i=0; i<numberOfBytes; i++)
    {
        printHex(data[i]);   
    }
}



uint8_t     data[256]  = {0};


void AXDRTests()
{
    //
    // Set
    //
    {
        AXDRStream  stream  = &data[0];
        uint32_t    valueOne       = 0xdeadbeef;
        uint8_t     stringOne[]    = {0x01,0x02,0x03,0x04,0x05};
        uint8_t     stringTwo[]    = {0x04,0x05,0x06};

        memset( &data[0], 0xaa, sizeof(data) );

        axdrSetArray(&stream, 1);
        axdrSetStruct(&stream, 3);
        axdrSetOctetString(&stream, stringOne,sizeof(stringOne));
        axdrSetOctetString(&stream, stringTwo,sizeof(stringTwo));
        axdrSetUint32(&stream, valueOne);
    }

    //
    // Get
    //
    {
        AXDRStream  stream  = &data[0];
        uint32_t    numberOfElements = 0;
        uint32_t    numberOfFields  = 0;
        uint32_t    valueOne        = 0;
        uint8_t     stringOne[32];
        uint32_t    stringOneLength = 0;
        uint8_t     stringTwo[32];
        uint32_t    stringTwoLength = 0;

        axdrGetArray(&stream, &numberOfElements);
        assert( numberOfElements == 1 );
        axdrGetStruct(&stream, &numberOfFields);
        assert( numberOfFields == 3 );
        axdrGetOctetString(&stream, &stringOne[0],sizeof(stringOne), &stringOneLength);
        axdrGetOctetString(&stream, &stringTwo[0],sizeof(stringTwo), &stringTwoLength);
        axdrGetUint32(&stream, &valueOne );

        printHexData( &stringOne[0], stringOneLength );
        printf("\n");
        printHexData( &stringTwo[0], stringTwoLength );
        printf("\n");
        printHexData( (void*)&valueOne, sizeof(valueOne) );
        printf("\n");
    }
}


void GetRequestTests()
{
    //
    // Form
    //
    {
        AXDRStream  stream  = &data[0];
        OBISCode    timeOBIS    = {0,0,1,0,0,255};

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormGetRequest( &stream, timeOBIS, TimeClass, 2 );
        dlmsFormSelectiveAccessType( &stream, NoSelectiveAccess );

        printHexData( &data[0], 40 );
        printf("\n");
    }

    //
    // Parse
    //
    {
        AXDRStream  stream  = &data[0];
        OBISCode        obisCode= {0};
        InterfaceClass  ic      = 0;
        AttributeId     attrId  = 0;

        dlmsParseGetRequest( &stream,  &obisCode, &ic, &attrId );
        assert( ic == TimeClass );
        assert( attrId == 2 );

        printHexData( &data[0], 40 );
        printf("\n");
    }
}


void GetResponseTests()
{
    {
        //
        // GetResponseNormal with a read-write-denied access result.
        //
        AXDRStream  stream  = &data[0];

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormGetResponseNormal( &stream,  AccessResult );

        DataAccessResult    result  = read_write_denied;
        axdrSetUint8( &stream, (uint8_t)result );

        printHexData( &data[0], 40 );
        printf("\n");
    }
    {
        //
        // GetResponseNormal
        //
        AXDRStream  stream  = &data[0];
        OBISCode        obisCode= {0};
        InterfaceClass  ic      = 0;
        AttributeId     attrId  = 0;
        ResultType      resultType;
        uint8_t         resultCode;
        DataAccessResult    result;

        dlmsParseGetResponseNormal( &stream,  &resultType );
        assert( resultType == AccessResult );

        axdrGetUint8( &stream, &resultCode );
        result  = resultCode;
        assert( result == read_write_denied );
    }

    {
        //
        // GetResponseNormal with a 12-byte octet string result.
        //
        AXDRStream  stream  = &data[0];

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormGetResponseNormal( &stream,  Data );

        uint8_t     timeResult[12]  = {1,2,3,4,5,6,7,8,9,10,11,12};
        axdrSetOctetString( &stream, timeResult,sizeof(timeResult) );

        printHexData( &data[0], 40 );
        printf("\n");
    }
    {
        //
        // GetResponseNormal
        //
        AXDRStream  stream  = &data[0];
        OBISCode        obisCode= {0};
        InterfaceClass  ic      = 0;
        AttributeId     attrId  = 0;
        ResultType      resultType;
        uint8_t         resultCode;

        dlmsParseGetResponseNormal( &stream,  &resultType );
        assert( resultType == Data );

        uint8_t         timeResult[16]      = {0};
        uint32_t        timeResultLength    = 0;
        axdrGetOctetString( &stream, &timeResult[0],sizeof(timeResult), &timeResultLength );
        assert(timeResultLength == 12);

        printHexData( &timeResult[0], timeResultLength );
        printf("\n");
    }
}



void SetRequestTests()
{
    {
        //
        // Form SetRequestNormal with a 12-byte octet string result.
        //
        AXDRStream  stream  = &data[0];
        OBISCode    timeOBIS    = {0,0,1,0,0,255};

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormSetRequest( &stream,  timeOBIS, TimeClass, 2 );
        dlmsFormSelectiveAccessType( &stream, NoSelectiveAccess );

        uint8_t     timeResult[12]  = {1,2,3,4,5,6,7,8,9,10,11,12};
        axdrSetOctetString( &stream, timeResult,sizeof(timeResult) );

        printHexData( &data[0], 40 );
        printf("\n");
    }
    {
        //
        // Parse SetRequestNormal
        //
        AXDRStream  stream  = &data[0];
        OBISCode        obisCode= {0};
        InterfaceClass  ic      = 0;
        AttributeId     attrId  = 0;
        SelectiveAccessType      saType;
        uint8_t         resultCode;
        OBISCode    timeOBIS    = {0,0,1,0,0,255};

        dlmsParseSetRequest( &stream, &ic, &obisCode, &attrId, &saType );
        assert( ic == TimeClass );
        assert( memcmp(obisCode, timeOBIS , 6) == 0 );
        assert( attrId == 2 );
        assert( saType == NoSelectiveAccess );

        uint8_t         timeResult[16]      = {0};
        uint32_t        timeResultLength    = 0;
        axdrGetOctetString( &stream, &timeResult[0],sizeof(timeResult), &timeResultLength );
        assert(timeResultLength == 12);

        printHexData( &timeResult[0], timeResultLength );
        printf("\n");
        printHexData( &obisCode[0], sizeof(OBISCode) );
        printf("\n");
    }
}


int main()
{

    AXDRTests();
    GetRequestTests();
    GetResponseTests();
    SetRequestTests();

    return 0;
}

