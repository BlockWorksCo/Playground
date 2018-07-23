






#include "axdr.h"
#include <stdio.h>
#include <string.h>



typedef enum
{
  DLMS_null_data           = 0,
  DLMS_array               = 1,
  DLMS_structure           = 2,
  DLMS_boolean             = 3,
  DLMS_bit_string          = 4,
  DLMS_integer32           = 5,
  DLMS_unsigned32          = 6,
  DLMS_octet_string        = 9,
  DLMS_visible_string      = 10,
  DLMS_bcd                 = 13,
  DLMS_integer8            = 15,
  DLMS_integer16           = 16,
  DLMS_unsigned8           = 17,
  DLMS_unsigned16          = 18,
  DLMS_compact_array       = 19,
  DLMS_integer64           = 20,
  DLMS_unsigned64          = 21,
  DLMS_enumerate           = 22,
  DLMS_float32             = 23,
  DLMS_float64             = 24,
  DLMS_date_time           = 25,
  DLMS_date                = 26,
  DLMS_time                = 27,
  DLMS_dont_care           = 255

} PrimitiveTypeTag;




//----------------------------------------------------------------------
// low-level buufer operations.

bool axdrStreamFromBytes( AXDRStream* stream, uint8_t* data, uint8_t numberOfBytes )
{
    stream->data    = data;
    stream->dataEnd = data + numberOfBytes;

    return true;
}

bool axdrGetByte( AXDRStream* stream, uint8_t* byte )
{
    *byte   = *stream->data;
    stream->data++;

    return true;
}

bool axdrGetByteAndCheck( AXDRStream* stream, uint8_t* byte, uint8_t expectedByte )
{
    uint8_t value   = *stream->data;

    if(byte != NULL) {
        *byte   = value;
    }

    if( value != expectedByte ) {
        return false;
    }

    stream->data++;

    return true;
}

bool axdrSetByte( AXDRStream* stream, uint8_t byte )
{
    *stream->data   = byte;
    stream->data++;

    return true;
}

bool axdrSetBytes( AXDRStream* stream, uint8_t* bytes, uint32_t numberOfBytes )
{
    memcpy(stream->data, bytes, numberOfBytes);
    stream->data+=numberOfBytes;

    return true;
}

//----------------------------------------------------------------------
// primitive types.

bool axdrSetUint8( AXDRStream* stream, uint8_t value )
{
    axdrSetByte( stream, DLMS_unsigned8 );
    axdrSetByte( stream, (value&0xff)>>0 );
}

bool axdrGetUint8( AXDRStream* stream, uint8_t* value )
{
    axdrGetByteAndCheck( stream, NULL, DLMS_unsigned8 );
    axdrGetByte( stream, value );
}


bool axdrSetUint16( AXDRStream* stream, uint16_t value )
{
    axdrSetByte( stream, DLMS_unsigned16 );
    axdrSetByte( stream, (value&0xff00)>>8 );
    axdrSetByte( stream, (value&0xff)>>0 );
}

bool axdrGetUint16( AXDRStream* stream, uint16_t* value )
{
    uint8_t byte;

    axdrGetByteAndCheck( stream, NULL, DLMS_unsigned16 );
    axdrGetByte( stream, &byte );
    *value   = (*value << 8) | (uint16_t)byte;
    axdrGetByte( stream, &byte );
    *value   = (*value << 8) | (uint16_t)byte;
}


bool axdrSetUint32( AXDRStream* stream, uint32_t value )
{
    axdrSetByte( stream, DLMS_unsigned32 );
    axdrSetByte( stream, (value&0xff000000)>>24 );
    axdrSetByte( stream, (value&0xff0000)>>16 );
    axdrSetByte( stream, (value&0xff00)>>8 );
    axdrSetByte( stream, (value&0xff)>>0 );
}

bool axdrGetUint32( AXDRStream* stream, uint32_t* value )
{
    uint8_t byte;

    axdrGetByteAndCheck( stream, NULL, DLMS_unsigned32 );
    axdrGetByte( stream, &byte );
    *value   = (*value << 8) | (uint32_t)byte;
    axdrGetByte( stream, &byte );
    *value   = (*value << 8) | (uint32_t)byte;
    axdrGetByte( stream, &byte );
    *value   = (*value << 8) | (uint32_t)byte;
    axdrGetByte( stream, &byte );
    *value   = (*value << 8) | (uint32_t)byte;
}






//----------------------------------------------------------------------
// Test app.

int main()
{
    AXDRStream  stream;
    uint8_t     data[]  = {0x7e, 0x00, 0x7e};

    axdrStreamFromBytes( &stream, &data[0], sizeof(data) );
}

