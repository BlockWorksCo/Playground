


#include "axdr.h"
#include "dlms.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>


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

void printPDU(uint8_t* startData, uint8_t* endData)
{
    ptrdiff_t    numberOfBytes   = (ptrdiff_t)endData - (ptrdiff_t)startData;

    printHexData( startData, numberOfBytes );
    printf("\r\n");
}



uint8_t     data[256]  = {0};

static OBISCode            timeOBIS    = {0,0,1,0,0,255};


void AXDRTests()
{
    //
    // Set
    //
    {
        Stream  stream         = &data[0];
        uint32_t    valueOne       = 0xdeadbeef;
        uint8_t     stringOne[]    = {0x01,0x02,0x03,0x04,0x05};
        uint8_t     stringTwo[]    = {0x04,0x05,0x06};

        memset( &data[0], 0xaa, sizeof(data) );

        axdrSetArray(&stream, 1);
        axdrSetStruct(&stream, 3);
        axdrSetOctetString(&stream, stringOne,sizeof(stringOne));
        axdrSetOctetString(&stream, stringTwo,sizeof(stringTwo));
        axdrSetUint32(&stream, valueOne);

        printPDU( &data[0], (uint8_t*)stream );
    }

    //
    // Get
    //
    {
        Stream  stream  = &data[0];
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
    }
}


void GetRequestTests()
{
    //
    // Form
    //
    {
        Stream  stream  = &data[0];

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormGetRequest( &stream, timeOBIS, TimeClass, 2 );
        dlmsFormNoAccessSelection( &stream );

        printPDU( &data[0], (uint8_t*)stream );
    }

    //
    // Parse
    //
    {
        Stream  stream  = &data[0];
        OBISCode        obisCode= {0};
        InterfaceClass  ic      = 0;
        AttributeId     attrId  = 1;
        bool            accessSelection = 0;
        uint8_t         accessSelector  = 0;

        dlmsParseGetRequest( &stream,  &obisCode, &ic, &attrId );
        assert( ic == TimeClass );
        assert( attrId == 2 );
        assert( accessSelector == 0 );

        dlmsParseAccessSelection( &stream, &accessSelection, &accessSelector );
        assert( accessSelection == false );
    }

    //
    // Form
    //
    {
        Stream  stream  = &data[0];

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormGetRequest( &stream, timeOBIS, TimeClass, 2 );
        dlmsFormByTimeRangeAccessSelection( &stream, 1234, 5678 );

        printPDU( &data[0], (uint8_t*)stream );
    }

}


void GetResponseTests()
{
    {
        //
        // GetResponseNormal with a read-write-denied access result.
        //
        Stream  stream  = &data[0];

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormGetResponseNormal( &stream,  AccessResult );

        DataAccessResult    result  = read_write_denied;
        axdrSetUint8( &stream, (uint8_t)result );

        printPDU( &data[0], (uint8_t*)stream );
    }
    {
        //
        // GetResponseNormal
        //
        Stream  stream  = &data[0];
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
        Stream  stream  = &data[0];

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormGetResponseNormal( &stream,  Data );

        uint8_t     timeResult[12]  = {1,2,3,4,5,6,7,8,9,10,11,12};
        axdrSetOctetString( &stream, timeResult,sizeof(timeResult) );

        printPDU( &data[0], (uint8_t*)stream );
    }
    {
        //
        // GetResponseNormal
        //
        Stream  stream  = &data[0];
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
    }
}



void SetRequestTests()
{
    {
        //
        // Form SetRequestNormal with a 12-byte octet string result.
        //
        Stream  stream  = &data[0];

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormSetRequest( &stream,  timeOBIS, TimeClass, 2 );
        dlmsFormNoAccessSelection( &stream );

        uint8_t     timeResult[]  = {1,2,3,4,5,6,7,8,9,10,11,12};
        axdrSetOctetString( &stream, timeResult,sizeof(timeResult) );

        uint8_t expected[]  = {0xC1,0x01,0xC1,0x08,0x00,0x00,0x00,0x01,0x00,0x00,0xFF,0x02,0x00,0x09,0x0C,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C};
        assert( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );

        printPDU( &data[0], (uint8_t*)stream );
    }
    {
        //
        // Parse SetRequestNormal
        //
        Stream          stream  = &data[0];
        OBISCode            obisCode= {0};
        InterfaceClass      ic      = 0;
        AttributeId         attrId  = 0;
        bool                accessSelection = false;
        uint8_t             accessSelector  = 0;
        uint8_t             resultCode;

        dlmsParseSetRequest( &stream, &ic, &obisCode, &attrId );
        assert( ic == TimeClass );
        assert( memcmp(obisCode, timeOBIS , 6) == 0 );
        assert( attrId == 2 );

        dlmsParseAccessSelection( &stream, &accessSelection, &accessSelector );
        assert( accessSelection == false );

        uint8_t         timeResult[16]      = {0};
        uint32_t        timeResultLength    = 0;
        axdrGetOctetString( &stream, &timeResult[0],sizeof(timeResult), &timeResultLength );
        assert(timeResult[0] == 1);
        assert(timeResult[11] == 12);
        assert(timeResultLength == 12);
    }
}


void SetResponseTests()
{
    {
        //
        // Form SetRequestNormal with a 12-byte octet string result.
        //
        Stream  stream  = &data[0];

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormSetResponse( &stream,  object_unavailable );

        printPDU( &data[0], (uint8_t*)stream );

        uint8_t expected[]  = {0xC5, 0x01, 0xc1, 0x0b };
        assert( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );

    }
    {
        //
        // Parse SetRequestNormal
        //
        Stream          stream  = &data[0];
        DataAccessResult    result;

        dlmsParseSetResponse( &stream, &result );
        assert( result == object_unavailable );
    }
}


int suiteInit(void)
{
    return 0;
}

int suiteDeinit(void)
{
    return 0;
}


int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", suiteInit, suiteDeinit);
    if (NULL == pSuite) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(pSuite, "AXDR tests", AXDRTests);
    CU_add_test(pSuite, "GetRequest tests", GetRequestTests);
    CU_add_test(pSuite, "GetResponse tests", GetResponseTests);
    CU_add_test(pSuite, "SetRequest tests", SetRequestTests);
    CU_add_test(pSuite, "SetResponse tests", SetResponseTests);

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

