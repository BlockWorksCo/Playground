


#ifndef __AXDR_H__
#define __AXDR_H__


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Stream.h"



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






void axdrSetUint8(AXDRStream* stream, uint8_t value);
void axdrSetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements);
void axdrSetLength(AXDRStream* stream, uint32_t length);
void axdrSetOctetString(AXDRStream* stream, uint8_t* data, uint32_t numberOfBytes);
void axdrSetUint32(AXDRStream* stream, uint32_t value);
void axdrSetUint16(AXDRStream* stream, uint16_t value);
void axdrSetUint8__(AXDRStream* stream, uint8_t value);  // TODO: Name clash!
void axdrSetStruct(AXDRStream* stream, uint32_t numberOfFields);
void axdrSetArray(AXDRStream* stream, uint32_t numberOfElements);
void axdrGetUint8(AXDRStream* stream, uint8_t* value);
void axdrGetLength(AXDRStream* stream, uint32_t* length);
void axdrGetArray(AXDRStream* stream, uint32_t* numberOfElements);
void axdrGetStruct(AXDRStream* stream, uint32_t* numberOfFields);
void axdrGetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements);
void axdrGetOctetString(AXDRStream* stream, uint8_t* data, uint32_t dataMaxSize, uint32_t* numberOfBytes);
void axdrGetUint32(AXDRStream* stream, uint32_t* value);
void axdrGetUint16(AXDRStream* stream, uint16_t* value);
void axdrGetUint8__(AXDRStream* stream, uint8_t* value);   // TODO: Name.

#endif

