


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


#if 1
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
#endif



static OBISCode            timeOBIS    = {0,0,1,0,0,255};


//
// Set
//
void AXDRTest1()
{
    uint8_t data[128]   = {0};
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

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0x01, 0x01, 0x02, 0x03, 0x09, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05, 0x09, 0x03, 0x04, 0x05, 0x06, 0x06, 0xEF, 0xBE, 0xAD, 0xDE};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );
}

//
// Get
//
void AXDRTest2()
{
    uint8_t input[]  = {0x01, 0x01, 0x02, 0x03, 0x09, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05, 0x09, 0x03, 0x04, 0x05, 0x06, 0x06, 0xEF, 0xBE, 0xAD, 0xDE};
    Stream  stream  = &input[0];
    uint32_t    numberOfElements = 0;
    uint32_t    numberOfFields  = 0;
    uint32_t    valueOne        = 0;
    uint8_t     stringOne[32];
    uint32_t    stringOneLength = 0;
    uint8_t     stringTwo[32];
    uint32_t    stringTwoLength = 0;

    axdrGetArray(&stream, &numberOfElements);
    CU_ASSERT( numberOfElements == 1 );

    axdrGetStruct(&stream, &numberOfFields);
    CU_ASSERT( numberOfFields == 3 );

    axdrGetOctetString(&stream, &stringOne[0],sizeof(stringOne), &stringOneLength);
    {
        uint8_t expected[]  = {0x01,0x02,0x03,0x04,0x05};
        CU_ASSERT( memcmp(&stringOne[0], &expected[0], sizeof(expected) ) == 0 );
    }

    axdrGetOctetString(&stream, &stringTwo[0],sizeof(stringTwo), &stringTwoLength);
    {
        uint8_t expected[]  = {0x04,0x05,0x06};
        CU_ASSERT( memcmp(&stringTwo[0], &expected[0], sizeof(expected) ) == 0 );
    }

    axdrGetUint32(&stream, &valueOne );
    CU_ASSERT( valueOne == 0xdeadbeef );
}


void GetRequestTest1()
{
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    memset( &data[0], 0xaa, sizeof(data) );
    dlmsFormGetRequest( &stream, timeOBIS, TimeClass, 2 );
    dlmsFormNoAccessSelection( &stream );

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0xC0, 0x01, 0x81, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x02, 0x00};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );
}

void GetRequestTest2()
{
    uint8_t input[]  = {0xC0, 0x01, 0x81, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x02, 0x00};
    Stream  stream  = &input[0];
    OBISCode        obisCode= {0};
    InterfaceClass  ic      = 0;
    AttributeId     attrId  = 1;
    bool            accessSelection = 0;
    uint8_t         accessSelector  = 0;

    dlmsParseGetRequest( &stream,  &obisCode, &ic, &attrId );
    CU_ASSERT( ic == TimeClass );
    CU_ASSERT( attrId == 2 );

    dlmsParseAccessSelection( &stream, &accessSelection, &accessSelector );
    CU_ASSERT( accessSelection == false );
    CU_ASSERT( accessSelector == 0 );
}

//
// Form
//
void GetRequestTest3()
{
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    memset( &data[0], 0xaa, sizeof(data) );
    dlmsFormGetRequest( &stream, timeOBIS, TimeClass, 2 );
    dlmsFormByTimeRangeAccessSelection( &stream, 1234, 5678 );

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0xC0, 0x01, 0x81, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x02, 0x01, 0x01, 0x02, 0x04, 0x02, 0x04, 0x12, 0x08, 0x00, 0x09, 0x06, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x11, 0x02, 0x12, 0x00, 0x00, 0x09, 0x0C, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x09, 0x0C, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x01, 0x00};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );
}

//
// Parse
//
void GetRequestTest4()
{
    uint8_t input[]  = {0xC0, 0x01, 0x81, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x02, 0x01, 0x01, 0x02, 0x04, 0x02, 0x04, 0x12, 0x08, 0x00, 0x09, 0x06, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x11, 0x02, 0x12, 0x00, 0x00, 0x09, 0x0C, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x09, 0x0C, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x01, 0x00};
    Stream  stream  = &input[0];
    OBISCode        obisCode= {0};
    InterfaceClass  ic      = 0;
    AttributeId     attrId  = 1;
    bool            accessSelection = 0;
    uint8_t         accessSelector  = 0;

    dlmsParseGetRequest( &stream,  &obisCode, &ic, &attrId );
    CU_ASSERT( ic == TimeClass );
    CU_ASSERT( attrId == 2 );

    dlmsParseAccessSelection( &stream, &accessSelection, &accessSelector );
    CU_ASSERT( accessSelection == true );
    CU_ASSERT( accessSelector == 1 );

    uint32_t    from    = 0;
    uint32_t    to      = 0;
    dlmsParseByTimeRangeAccessSelection( &stream, &from, &to );
    //CU_ASSERT( from == 1234 );
    //CU_ASSERT( to == 5678 );
}

//
// Form
//
void GetRequestTest5()
{
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    memset( &data[0], 0xaa, sizeof(data) );
    dlmsFormGetRequest( &stream, timeOBIS, TimeClass, 2 );
    dlmsFormByEntryAccessSelection( &stream, 1234, 5678 );

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0xC0, 0x01, 0x81, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x02, 0x01, 0x02, 0x02, 0x04, 0x06, 0xD2, 0x04, 0x00, 0x00, 0x06, 0x2E, 0x16, 0x00, 0x00, 0x12, 0x00, 0x00, 0x12, 0x00, 0x00};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );
}

//
// Parse
//
void GetRequestTest6()
{
    uint8_t input[]  = {0xC0, 0x01, 0x81, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x02, 0x01, 0x02, 0x02, 0x04, 0x06, 0xD2, 0x04, 0x00, 0x00, 0x06, 0x2E, 0x16, 0x00, 0x00, 0x12, 0x00, 0x00, 0x12, 0x00, 0x00};
    Stream  stream  = &input[0];
    OBISCode        obisCode= {0};
    InterfaceClass  ic      = 0;
    AttributeId     attrId  = 1;
    bool            accessSelection = 0;
    uint8_t         accessSelector  = 0;

    dlmsParseGetRequest( &stream,  &obisCode, &ic, &attrId );
    CU_ASSERT( ic == TimeClass );
    CU_ASSERT( attrId == 2 );

    dlmsParseAccessSelection( &stream, &accessSelection, &accessSelector );
    CU_ASSERT( accessSelection == true );
    CU_ASSERT( accessSelector == 2 );

    uint32_t    from    = 0;
    uint32_t    to      = 0;
    dlmsParseByEntryAccessSelection( &stream, &from, &to );
    //CU_ASSERT( from == 1234 );
    //CU_ASSERT( to == 5678 );
}


void GetResponseTest1()
{
    //
    // GetResponseNormal with a read-write-denied access result.
    //
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    memset( &data[0], 0xaa, sizeof(data) );
    dlmsFormGetResponseNormal( &stream,  AccessResult );

    DataAccessResult    result  = read_write_denied;
    axdrSetUint8( &stream, (uint8_t)result );

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0xC4, 0x01, 0x81, 0x01, 0x11, 0x03};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );
}

void GetResponseTest2()
{
    //
    // GetResponseNormal
    //
    uint8_t input[]  = {0xC4, 0x01, 0x81, 0x01, 0x11, 0x03};
    Stream  stream  = &input[0];
    OBISCode        obisCode= {0};
    InterfaceClass  ic      = 0;
    AttributeId     attrId  = 0;
    ResultType      resultType;
    uint8_t         resultCode;
    DataAccessResult    result;

    dlmsParseGetResponseNormal( &stream,  &resultType );
    CU_ASSERT( resultType == AccessResult );

    axdrGetUint8( &stream, &resultCode );
    result  = resultCode;
    CU_ASSERT( result == read_write_denied );
}

void GetResponseTest3()
{
    //
    // GetResponseNormal with a 12-byte octet string result.
    //
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    memset( &data[0], 0xaa, sizeof(data) );
    dlmsFormGetResponseNormal( &stream,  Data );

    uint8_t     timeResult[12]  = {1,2,3,4,5,6,7,8,9,10,11,12};
    axdrSetOctetString( &stream, timeResult,sizeof(timeResult) );

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0xC4, 0x01, 0x81, 0x00, 0x09, 0x0C, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );
}

void GetResponseTest4()
{
    //
    // GetResponseNormal
    //
    uint8_t input[]  = {0xC4, 0x01, 0x81, 0x00, 0x09, 0x0C, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C};
    Stream  stream  = &input[0];
    OBISCode        obisCode= {0};
    InterfaceClass  ic      = 0;
    AttributeId     attrId  = 0;
    ResultType      resultType;
    uint8_t         resultCode;

    dlmsParseGetResponseNormal( &stream,  &resultType );
    CU_ASSERT( resultType == Data );

    uint8_t         timeResult[16]      = {0};
    uint32_t        timeResultLength    = 0;
    axdrGetOctetString( &stream, &timeResult[0],sizeof(timeResult), &timeResultLength );
    CU_ASSERT(timeResultLength == 12);
}



void SetRequestTest1()
{
    //
    // Form SetRequestNormal with a 12-byte octet string result.
    //
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    memset( &data[0], 0xaa, sizeof(data) );
    dlmsFormSetRequest( &stream,  timeOBIS, TimeClass, 2 );
    dlmsFormNoAccessSelection( &stream );

    uint8_t     timeResult[]  = {1,2,3,4,5,6,7,8,9,10,11,12};
    axdrSetOctetString( &stream, timeResult,sizeof(timeResult) );

    uint8_t expected[]  = {0xC1,0x01,0xC1,0x08,0x00,0x00,0x00,0x01,0x00,0x00,0xFF,0x02,0x00,0x09,0x0C,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );

    //printPDU( &data[0], (uint8_t*)stream );
}

void SetRequestTest2()
{
    //
    // Parse SetRequestNormal
    //
    uint8_t         input[]  = {0xC1,0x01,0xC1,0x08,0x00,0x00,0x00,0x01,0x00,0x00,0xFF,0x02,0x00,0x09,0x0C,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C};
    Stream          stream  = &input[0];
    OBISCode            obisCode= {0};
    InterfaceClass      ic      = 0;
    AttributeId         attrId  = 0;
    bool                accessSelection = false;
    uint8_t             accessSelector  = 0;
    uint8_t             resultCode;

    dlmsParseSetRequest( &stream, &ic, &obisCode, &attrId );
    CU_ASSERT( ic == TimeClass );
    CU_ASSERT( memcmp(obisCode, timeOBIS , 6) == 0 );
    CU_ASSERT( attrId == 2 );

    dlmsParseAccessSelection( &stream, &accessSelection, &accessSelector );
    CU_ASSERT( accessSelection == false );

    uint8_t         timeResult[16]      = {};
    uint32_t        timeResultLength    = 0;
    axdrGetOctetString( &stream, &timeResult[0],sizeof(timeResult), &timeResultLength );
    CU_ASSERT(timeResult[0] == 1);
    CU_ASSERT(timeResult[11] == 12);
    CU_ASSERT(timeResultLength == 12);
}


void SetResponseTest1()
{
    //
    // Form .
    //
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    memset( &data[0], 0xaa, sizeof(data) );
    dlmsFormSetResponse( &stream,  object_unavailable );

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0xC5, 0x01, 0xc1, 0x0b };
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );

}

void SetResponseTest2()
{
    //
    // Parse .
    //
    uint8_t input[]  = {0xC5, 0x01, 0xc1, 0x0b };
    Stream          stream  = &input[0];
    DataAccessResult    result;

    dlmsParseSetResponse( &stream, &result );
    CU_ASSERT( result == object_unavailable );
}


void GenerateLLSAARQ()
{
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    dlmsFormAARQForLLS( &stream );

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0x60, 0x36, 0xA1, 0x09, 0x06, 0x07, 0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x01, 0x8A, 0x02, 0x07, 0x80, 0x8B, 0x07, 0x60, 0x85, 0x74, 0x05, 0x08, 0x02, 0x01, 0xAC, 0x0A, 0x80, 0x09, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x00, 0xBE, 0x10, 0x04, 0x0E, 0x01, 0x00, 0x00, 0x00, 0x06, 0x5F, 0x1F, 0x04, 0x00, 0x00, 0x7E, 0x1F, 0x04, 0xB0};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );
}



void GenerateHLSAARQ()
{
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    dlmsFormAARQForHLS( &stream );

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0x60, 0x36, 0xA1, 0x09, 0x06, 0x07, 0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x01, 0x8A, 0x02, 0x07, 0x80, 0x8B, 0x07, 0x60, 0x85, 0x74, 0x05, 0x08, 0x02, 0x05, 0xAC, 0x0A, 0x80, 0x09, 0x4B, 0x35, 0x36, 0x69, 0x56, 0x61, 0x67, 0x59, 0x00, 0xBE, 0x10, 0x04, 0x0E, 0x01, 0x00, 0x00, 0x00, 0x06, 0x5F, 0x1F, 0x04, 0x00, 0x00, 0x7E, 0x1F, 0x04, 0xB0};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );
}

void GenerateInitiateRequestAPDU()
{
    uint8_t data[128]   = {0};
    Stream  stream  = &data[0];

    uint8_t dedicatedKey[]  = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    dlmsFormInitiateRequestAPDU( &stream, dedicatedKey,sizeof(dedicatedKey), false, false, 0x06, 0x007e1f, 1200 );

    //printPDU( &data[0], (uint8_t*)stream );

    uint8_t expected[]  = {0x01, 0x01, 0x10, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x00, 0x06, 0x5F, 0x1F, 0x04, 0x00, 0x00, 0x7E, 0x1F, 0x04, 0xB0};
    CU_ASSERT( memcmp(&data[0], &expected[0], sizeof(expected) ) == 0 );

}

void ParseAARQ()
{
    uint8_t input[]  = {0x60, 0x36, 0xA1, 0x09, 0x06, 0x07, 0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x01, 0x8A, 0x02, 0x07, 0x80, 0x8B, 0x07, 0x60, 0x85, 0x74, 0x05, 0x08, 0x02, 0x05, 0xAC, 0x0A, 0x80, 0x09, 0x4B, 0x35, 0x36, 0x69, 0x56, 0x61, 0x67, 0x59, 0x00, 0xBE, 0x10, 0x04, 0x0E, 0x01, 0x00, 0x00, 0x00, 0x06, 0x5F, 0x1F, 0x04, 0x00, 0x00, 0x7E, 0x1F, 0x04, 0xB0};
    Stream  stream  = &input[0];

    uint8_t mechanismName[32]   = {0};
    dlmsParseAARQ( &stream, &mechanismName[0], sizeof(mechanismName) );
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

    CU_add_test(pSuite, "AXDR test1", AXDRTest1);
    CU_add_test(pSuite, "AXDR test2", AXDRTest2);
    CU_add_test(pSuite, "GetRequest test1", GetRequestTest1);
    CU_add_test(pSuite, "GetRequest test2", GetRequestTest2);
    CU_add_test(pSuite, "GetRequest test3", GetRequestTest3);
    CU_add_test(pSuite, "GetRequest test4", GetRequestTest4);
    CU_add_test(pSuite, "GetRequest test5", GetRequestTest5);
    CU_add_test(pSuite, "GetRequest test6", GetRequestTest6);
    CU_add_test(pSuite, "GetResponse tests", GetResponseTest1);
    CU_add_test(pSuite, "GetResponse tests", GetResponseTest2);
    CU_add_test(pSuite, "GetResponse tests", GetResponseTest3);
    CU_add_test(pSuite, "GetResponse tests", GetResponseTest4);
    CU_add_test(pSuite, "SetRequest1 tests", SetRequestTest1);
    CU_add_test(pSuite, "SetRequest2 tests", SetRequestTest2);
    CU_add_test(pSuite, "SetResponse1 tests", SetResponseTest1);
    CU_add_test(pSuite, "SetResponse2 tests", SetResponseTest2);
    CU_add_test(pSuite, "Generate LLS AARQ", GenerateLLSAARQ);
    CU_add_test(pSuite, "Generate HLS AARQ", GenerateHLSAARQ);
    CU_add_test(pSuite, "Generate InitiateRequest APDU", GenerateInitiateRequestAPDU);
    CU_add_test(pSuite, "ParseAARQ", ParseAARQ);


    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

