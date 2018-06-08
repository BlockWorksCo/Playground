


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*
Data ::= CHOICE
{
  null-data            [0]   IMPLICIT NULL,
  array                [1]   IMPLICIT SEQUENCE OF Data,
  structure            [2]   IMPLICIT SEQUENCE OF Data,
  bool                 [3]   IMPLICIT BOOLEAN ,
  bit-string           [4]   IMPLICIT BIT STRING ,
  double-long          [5]   IMPLICIT Integer32,
  double-long-unsigned [6]   IMPLICIT Unsigned32,
  octet-string         [9]   IMPLICIT OCTET STRING ,
  visible-string       [10]  IMPLICIT VisibleString,
  bcd                  [13]  IMPLICIT Integer8,
  integer              [15]  IMPLICIT Integer8,
  long-integer         [16]  IMPLICIT Integer16,
  unsigned             [17]  IMPLICIT Unsigned8,
  long-unsigned        [18]  IMPLICIT Unsigned16,
  compact-array        [19]  IMPLICIT SEQUENCE
  {
    contents-description [0] TypeDescription,
    array-contents       [1] IMPLICIT OCTET STRING
  },
  long64               [20]  IMPLICIT Integer64,
  long64-unsigned      [21]  IMPLICIT Unsigned64,
  enumerate            [22]  IMPLICIT Enum,
  float32              [23]  IMPLICIT OCTET STRING (SIZE(4)),
  float64              [24]  IMPLICIT OCTET STRING (SIZE(8)),
  date-time            [25]  IMPLICIT OCTET STRING (SIZE(12)),
  date                 [26]  IMPLICIT OCTET STRING (SIZE(5)),
  time                 [27]  IMPLICIT OCTET STRING (SIZE(4)),
  dont-care            [255] IMPLICIT NULL
}
*/


typedef enum
{
  null_data           = 0,
  array               = 1,
  structure           = 2,
  boolean             = 3,
  bit_string          = 4,
  integer32           = 5,
  unsigned32          = 6,
  octet_string        = 9,
  visible_string      = 10,
  bcd                 = 13,
  integer8            = 15,
  integer16           = 16,
  unsigned8           = 17,
  unsigned16          = 18,
  compact_array       = 19,
  integer64           = 20,
  unsigned64          = 21,
  enumerate           = 22,
  float32             = 23,
  float64             = 24,
  date_time           = 25,
  date                = 26,
  time                = 27,
  dont_care           = 255

} PrimitiveTypeTag;


typedef struct
{
    uint8_t type;
    union
    {
        uint8_t     null_data;
        uint8_t     array;
        uint8_t     structure;
        uint8_t     boolean;
        uint8_t     bit_string[1];
        int32_t     integer32;
        uint32_t    unsigned32;
        uint8_t     octet_string[1];
        uint8_t     visible_string[1];
        uint8_t     bcd;
        uint8_t     integer8;
        int16_t     integer16;
        uint8_t     unsigned8;
        uint16_t    unsigned16;
        uint8_t     compact_array;
        int64_t     integer64;
        uint64_t    unsigned64;
        uint8_t     enumerate;
        float       float32;
        double      float64;
        uint8_t     date_time;
        uint8_t     date;
        uint8_t     time;
        uint8_t     dont_care;

    } value;

} __attribute__((packed)) PrimitiveType;


static const int32_t     primitiveTypeLength[]   =
{
  -1,   // null_data           = 0,
  -1,   // array               = 1,
  -1,   // structure           = 2,
  1,    // boolean             = 3,
  -1,   // bit_string          = 4,
  4,    // integer32           = 5,
  4,    // unsigned32          = 6,
  -1,   // octet_string        = 9,
  -1,   // visible_string      = 10,
  1,    // bcd                 = 13,
  1,    // integer8            = 15,
  2,    // integer16           = 16,
  1,    // unsigned8           = 17,
  2,    // unsigned16          = 18,
  -1,   // compact_array       = 19,
  8,    // integer64           = 20,
  8,    // unsigned64          = 21,
  1,    // enumerate           = 22,
  4,    // float32             = 23,
  8,    // float64             = 24,
  12,   // date_time           = 25,
  4,    // date                = 26,
  4,    // time                = 27,
  -1,   // dont_care           = 255
};



typedef enum
{
    TimeClass   = 8,

} InterfaceClass;


typedef enum
{
    Data            = 0,
    AccessResult    = 1, 

} ResultType;


typedef enum
{
    success                   = 0,
    hardware_fault            = 1,
    temporary_failure         = 2,
    read_write_denied         = 3,
    object_undefined          = 4,
    object_class_inconsistent = 9,
    object_unavailable        = 11,
    type_unmatched            = 12,
    scope_of_access_violated  = 13,
    data_block_unavailable    = 14,
    long_get_aborted          = 15,
    no_long_get_in_progress   = 16,
    long_set_aborted          = 17,
    no_long_set_in_progress   = 18,
    other_reason              = 250

} DataAccessResult;


typedef enum
{
    NoSelectiveAccess,

} SelectiveAccessType;

typedef uint8_t*    AXDRStream;
typedef uint8_t     OBISCode[6];
typedef uint16_t    AttributeId;


void dlmsFormAttributeDescriptor( AXDRStream* stream, InterfaceClass ifClass, OBISCode obisCode, AttributeId attrId );
void dlmsParseAttributeDescriptor( AXDRStream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId );







void axdrSetUint8(AXDRStream* stream, uint8_t value)
{
    uint8_t*    streamBytes = (uint8_t*)*stream;
    streamBytes[0]  = value;
    *stream      = (AXDRStream)(streamBytes+1);
}

void axdrSetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements)
{
    for(uint32_t i=0; i<numberOfElements; i++)
    {
        axdrSetUint8( stream, values[i] );
    }
}

void axdrSetLength(AXDRStream* stream, uint32_t length)
{
    uint8_t*    streamBytes = (uint8_t*)*stream;
    
    if(length < 0x80)
    {
        streamBytes[0]  = (uint8_t)length;
        *stream      = (AXDRStream)(streamBytes+1);
    }
    else if(length < 0xffff)
    {
        streamBytes[0]  = 0x82;
        streamBytes[1]  = (length>>8)&0x00ff;
        streamBytes[2]  = length&0x00ff;
        *stream      = (AXDRStream)(streamBytes+3);
    }
    else
    {
        streamBytes[0]  = 0x84;
        streamBytes[1]  = (length>>24)&0x000000ff;
        streamBytes[2]  = (length>>16)&0x000000ff;
        streamBytes[3]  = (length>>8)&0x000000ff;
        streamBytes[4]  = length&0x000000ff;
        *stream      = (AXDRStream)(streamBytes+5);
    }
}

void axdrSetOctetString(AXDRStream* stream, uint8_t* data, uint32_t numberOfBytes)
{
    axdrSetUint8(stream, octet_string);
    axdrSetLength(stream, numberOfBytes);
    axdrSetUint8Array(stream, data,numberOfBytes);
}


void axdrSetUint32(AXDRStream* stream, uint32_t value)
{
    axdrSetUint8(stream, unsigned32);
    axdrSetUint8Array(stream, (void*)&value,sizeof(value) );
}


void axdrSetUint16(AXDRStream* stream, uint16_t value)
{
    axdrSetUint8(stream, unsigned16);
    axdrSetUint8Array(stream, (void*)&value,sizeof(value) );
}


void axdrSetStruct(AXDRStream* stream, uint32_t numberOfFields)
{
    axdrSetUint8(stream, structure);
    axdrSetLength(stream, numberOfFields);
}


void axdrSetArray(AXDRStream* stream, uint32_t numberOfElements)
{
    axdrSetUint8(stream, array);
    axdrSetLength(stream, numberOfElements);
}





void axdrGetUint8(AXDRStream* stream, uint8_t* value)
{
    uint8_t*    streamBytes = (uint8_t*)*stream;
    
    *value  = streamBytes[0];
    *stream  = (AXDRStream)(streamBytes+1);
}

void axdrGetLength(AXDRStream* stream, uint32_t* length)
{
    uint8_t*    streamBytes = (uint8_t*)*stream;
    
    uint8_t     value   = streamBytes[0];
    if( (value&0x80) == 0)
    {
        *length = value;
        *stream  = (AXDRStream)(streamBytes+1);
    }
    else
    {
        uint8_t numberOfBytes   = value & 0x7f;
        if(numberOfBytes == 2)
        {
            *length = (streamBytes[1]<<8) | (streamBytes[2]);
            *stream  = (AXDRStream)(streamBytes+3);
        }
        else if(numberOfBytes == 4)
        {
            *length = (streamBytes[1]<<24) | (streamBytes[2]<<16) | (streamBytes[3]<<8) | (streamBytes[4]);
            *stream  = (AXDRStream)(streamBytes+5);
        }
        else
        {
            *stream  = (AXDRStream)(streamBytes+1);
        }
    }
}

void axdrGetArray(AXDRStream* stream, uint32_t* numberOfElements)
{
    uint8_t    tag   = 0;
    axdrGetUint8( stream, &tag );
    assert(tag == array);

    axdrGetLength( stream, numberOfElements );
}

void axdrGetStruct(AXDRStream* stream, uint32_t* numberOfFields)
{
    uint8_t    tag   = 0;
    axdrGetUint8( stream, &tag );
    assert(tag == structure);

    axdrGetLength( stream, numberOfFields );
}

void axdrGetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements)
{
    for(uint32_t i=0; i<numberOfElements; i++)
    {
        axdrGetUint8( stream, &values[i] );
    }
}


void axdrGetOctetString(AXDRStream* stream, uint8_t* data, uint32_t dataMaxSize, uint32_t* numberOfBytes)
{
    uint8_t    tag   = 0;
    axdrGetUint8( stream, &tag );
    assert(tag == octet_string);

    axdrGetLength( stream, numberOfBytes );
    assert( *numberOfBytes < dataMaxSize );

    axdrGetUint8Array( stream, data,*numberOfBytes );
}

void axdrGetUint32(AXDRStream* stream, uint32_t* value)
{
    uint8_t    tag   = 0;
    axdrGetUint8( stream, &tag );
    assert(tag == unsigned32);

    axdrGetUint8Array( stream, (void*)value,sizeof(*value) );
}


void axdrGetUint16(AXDRStream* stream, uint16_t* value)
{
    uint8_t    tag   = 0;
    axdrGetUint8( stream, &tag );
    assert(tag == unsigned16);

    axdrGetUint8Array( stream, (void*)value,sizeof(*value) );
}







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
void dlmsFormGetRequest( AXDRStream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId )
{
    axdrSetUint8( stream, 0xc0 );   // type
    axdrSetUint8( stream, 0x01 );   // subType
    axdrSetUint8( stream, 0x81 );   // invokeId
    
    dlmsFormAttributeDescriptor( stream, ifClass, obisCode, attributeId );
}

void dlmsParseGetRequest( AXDRStream* stream,  OBISCode* obisCode, InterfaceClass* ifClass, AttributeId* attributeId )
{
    uint8_t     type    = 0;
    uint8_t     subType = 0;
    uint8_t     invokeId= 0;
    uint16_t    ic      = 0;
    uint16_t    attr    = 0;

    axdrGetUint8( stream, &type );
    assert( type == 0xc0 );

    axdrGetUint8( stream, &subType );
    assert( subType == 0x01 );

    axdrGetUint8( stream, &invokeId );

    dlmsParseAttributeDescriptor( stream, ifClass, obisCode, attributeId );
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
    axdrSetUint8( stream, 0xc4 );   // type
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
    assert( type == 0xc4 );

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
void dlmsFormSetRequest( AXDRStream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId )
{
    axdrSetUint8( stream, 0xc1 );   // type
    axdrSetUint8( stream, 0x01 );   // subType
    axdrSetUint8( stream, 0xc1 );   // invokeId
    
    uint16_t    ic  = ifClass;
    axdrSetUint8Array( stream, (void*)&ic, sizeof(ic) );

    axdrSetUint8Array( stream, obisCode,sizeof(OBISCode) );
    
    uint8_t     attrId  = attributeId;
    axdrSetUint8( stream, attrId );
}

void dlmsFormSelectiveAccessType( AXDRStream* stream, SelectiveAccessType type)
{
    uint8_t     saType  = type;
    axdrSetUint8( stream, type );
}

void dlmsParseSetRequest( AXDRStream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId, SelectiveAccessType* saType )
{
    uint8_t     type    = 0;
    uint8_t     subType = 0;
    uint8_t     invokeId= 0;
    uint8_t     result  = 0;
    uint16_t    ic      = 0;
    uint8_t     sa      = 0;
    uint8_t     attr    = 0;

    axdrGetUint8( stream, &type );
    assert( type == 0xc1 );

    axdrGetUint8( stream, &subType );
    assert( subType == 0x01 );

    axdrGetUint8( stream, &invokeId );

    axdrGetUint8Array( stream, (void*)&ic,sizeof(ic) );
    *ifClass    = (InterfaceClass)ic;

    axdrGetUint8Array( stream, (void*)obisCode,sizeof(OBISCode) );

    axdrGetUint8( stream, &attr );
    *attrId  = (AttributeId)attr;

    axdrGetUint8( stream, &sa );
    *saType   = (SelectiveAccessType)sa;
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

int main()
{
    uint8_t     data[1024]  = {0};

    {
        //
        // AXDR test routines.
        //
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

    printHexData( &data[0], 40 );
    printf("\n");

    {
        //
        // AXDR test routines.
        //
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


    {
        //
        // GetRequestNormal
        //
        AXDRStream  stream  = &data[0];
        OBISCode    timeOBIS    = {0,0,1,0,0,255};

        memset( &data[0], 0xaa, sizeof(data) );
        dlmsFormGetRequest( &stream, timeOBIS, TimeClass, 2 );
        dlmsFormSelectiveAccessType( &stream, NoSelectiveAccess );

        printHexData( &data[0], 40 );
        printf("\n");
    }
    {
        //
        // GetRequestNormal
        //
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


    {
        //
        // SetRequestNormal with a 12-byte octet string result.
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
        // SetRequestNormal
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




    return 0;
}

