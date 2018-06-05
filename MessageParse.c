


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



typedef uint8_t     AXDRType;
typedef uint16_t    AXDRLength;
typedef uint8_t*    AXDRStream;


void axdrGetTypeAndLength(AXDRStream* stream, AXDRType* type, AXDRLength* length)
{
    uint8_t*    streamBytes = (uint8_t*)stream;
    *type   = *streamBytes;
    streamBytes++;
    PrimitiveTypeTag    tag = (PrimitiveTypeTag)*type;
    int32_t     typeLength  = primitiveTypeLength[tag];
    if(typeLength == -1)
    {
        *length = *streamBytes;
        streamBytes++;
    }
    else
    {
        *length = typeLength;
    }

    *stream = streamBytes;
}

void axdrGetTypeAndValue(AXDRStream* stream)
{
}

void axdrGetUINT32(AXDRStream stream, uint32_t* value)
{
}



void axdrSetUint8(AXDRStream inStream, AXDRStream* outStream, uint8_t value)
{
    uint8_t*    streamBytes = (uint8_t*)inStream;
    streamBytes[0]  = value;
    *outStream      = (AXDRStream)(streamBytes+1);
}

void axdrSetUint8Array(AXDRStream inStream, AXDRStream* outStream, uint8_t* values, uint32_t numberOfElements)
{
    for(uint32_t i=0; i<numberOfElements; i++)
    {
        axdrSetUint8( inStream, &inStream, values[i] );
    }
    *outStream  = inStream;
}

void axdrSetLength(AXDRStream inStream, AXDRStream* outStream, uint32_t length)
{
    uint8_t*    streamBytes = (uint8_t*)inStream;
    
    if(length < 0x80)
    {
        streamBytes[0]  = (uint8_t)length;
        *outStream      = (AXDRStream)(streamBytes+1);
    }
    else if(length < 0xffff)
    {
        streamBytes[0]  = 0x82;
        streamBytes[1]  = (length>>8)&0x00ff;
        streamBytes[2]  = length&0x00ff;
        *outStream      = (AXDRStream)(streamBytes+3);
    }
    else
    {
        streamBytes[0]  = 0x84;
        streamBytes[1]  = (length>>24)&0x000000ff;
        streamBytes[2]  = (length>>16)&0x000000ff;
        streamBytes[3]  = (length>>8)&0x000000ff;
        streamBytes[4]  = length&0x000000ff;
        *outStream      = (AXDRStream)(streamBytes+5);
    }
}

void axdrSetOctetString(AXDRStream inStream, AXDRStream* outStream, uint8_t* data, uint32_t numberOfBytes)
{
    axdrSetUint8(inStream, &inStream, octet_string);
    axdrSetLength(inStream, &inStream, numberOfBytes);
    axdrSetUint8Array(inStream, &inStream, data,numberOfBytes);
    *outStream  = inStream;
}


void axdrSetUint32(AXDRStream inStream, AXDRStream* outStream, uint32_t value)
{
    axdrSetUint8(inStream, &inStream, unsigned32);
    axdrSetUint8Array(inStream, &inStream, (void*)&value,sizeof(value) );
    *outStream  = inStream;
}


void axdrSetStruct(AXDRStream inStream, AXDRStream* outStream, uint32_t numberOfFields)
{
    axdrSetUint8(inStream, &inStream, structure);
    axdrSetLength(inStream, &inStream, numberOfFields);
    *outStream  = inStream;
}





void axdrGetUint8(AXDRStream inStream, AXDRStream* outStream, uint8_t* value)
{
    uint8_t*    streamBytes = (uint8_t*)inStream;
    
    *value  = streamBytes[0];
    *outStream  = (AXDRStream)(streamBytes+1);
}

void axdrGetLength(AXDRStream inStream, AXDRStream* outStream, uint32_t* length)
{
    uint8_t*    streamBytes = (uint8_t*)inStream;
    
    uint8_t     value   = streamBytes[0];
    if( (value&0x80) == 0)
    {
        *length = value;
        *outStream  = (AXDRStream)(streamBytes+1);
    }
    else
    {
        uint8_t numberOfBytes   = value & 0x7f;
        if(numberOfBytes == 2)
        {
            *length = (streamBytes[1]<<8) | (streamBytes[2]);
            *outStream  = (AXDRStream)(streamBytes+3);
        }
        else if(numberOfBytes == 4)
        {
            *length = (streamBytes[1]<<24) | (streamBytes[2]<<16) | (streamBytes[3]<<8) | (streamBytes[2]);
            *outStream  = (AXDRStream)(streamBytes+5);
        }
        else
        {
            *outStream  = (AXDRStream)(streamBytes+1);
        }
    }
}

void axdrGetStruct(AXDRStream inStream, AXDRStream* outStream, uint32_t* numberOfFields)
{
    uint8_t    tag   = 0;
    axdrGetUint8( inStream, &inStream, &tag );
    assert(tag == structure);

    axdrGetLength( inStream, &inStream, numberOfFields );
    
    *outStream  = inStream;
}

void axdrGetUint8Array(AXDRStream inStream, AXDRStream* outStream, uint8_t* values, uint32_t numberOfElements)
{
    for(uint32_t i=0; i<numberOfElements; i++)
    {
        axdrGetUint8( inStream, &inStream, &values[i] );
    }
    *outStream  = inStream;
}


void axdrGetOctetString(AXDRStream inStream, AXDRStream* outStream, uint8_t* data, uint32_t dataMaxSize, uint32_t* numberOfBytes)
{
    uint8_t    tag   = 0;
    axdrGetUint8( inStream, &inStream, &tag );
    assert(tag == octet_string);

    axdrGetLength( inStream, &inStream, numberOfBytes );

    axdrGetUint8Array( inStream, &inStream, data,*numberOfBytes );
    
    *outStream  = inStream;
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
        AXDRStream  stream  = &data[0];
        uint8_t     stringOne[]    = {0x01,0x02,0x03,0x04,0x05};
        uint8_t     stringTwo[]    = {0x04,0x05,0x06};

        axdrSetStruct(stream, &stream, 2);
        axdrSetOctetString(stream, &stream, stringOne,sizeof(stringOne));
        axdrSetOctetString(stream, &stream, stringTwo,sizeof(stringTwo));
    }

    printHexData( &data[0], 40 );
    printf("\n");

    {
        AXDRStream  stream  = &data[0];
        bool        completeFlag    = false;
        uint32_t    numberOfFields  = 0;
        uint8_t     stringOne[32];
        uint32_t    stringOneLength = 0;
        uint8_t     stringTwo[32];
        uint32_t    stringTwoLength = 0;

        axdrGetStruct(stream, &stream, &numberOfFields);
        assert( numberOfFields == 2 );
        axdrGetOctetString(stream, &stream, &stringOne[0],sizeof(stringOne), &stringOneLength);
        axdrGetOctetString(stream, &stream, &stringTwo[0],sizeof(stringTwo), &stringTwoLength);

        printHexData( &stringOne[0], stringOneLength );
        printf("\n");
        printHexData( &stringTwo[0], stringTwoLength );
        printf("\n");
    }

    return 0;
}

