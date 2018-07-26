






#include "axdr.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>



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

bool axdrGetBytes( AXDRStream* stream, uint8_t* bytes, uint32_t numberOfBytes )
{
    memcpy(bytes, stream->data, numberOfBytes);
    stream->data    += numberOfBytes;

    return true;
}

//----------------------------------------------------------------------
// primitive types.

bool axdrSetUint8( AXDRStream* stream, uint8_t value )
{
    axdrSetByte( stream, DLMS_unsigned8 );
    axdrSetByte( stream, (value&0xff)>>0 );

    return true;
}

bool axdrGetUint8( AXDRStream* stream, uint8_t* value )
{
    axdrGetByteAndCheck( stream, NULL, DLMS_unsigned8 );
    axdrGetByte( stream, value );

    return true;
}


bool axdrSetUint16( AXDRStream* stream, uint16_t value )
{
    axdrSetByte( stream, DLMS_unsigned16 );
    axdrSetByte( stream, (value&0xff00)>>8 );
    axdrSetByte( stream, (value&0xff)>>0 );

    return true;
}

bool axdrGetUint16( AXDRStream* stream, uint16_t* value )
{
    uint8_t byte;

    axdrGetByteAndCheck( stream, NULL, DLMS_unsigned16 );
    axdrGetByte( stream, &byte );
    *value   = (*value << 8) | (uint16_t)byte;
    axdrGetByte( stream, &byte );
    *value   = (*value << 8) | (uint16_t)byte;

    return true;
}


bool axdrSetUint32( AXDRStream* stream, uint32_t value )
{
    axdrSetByte( stream, DLMS_unsigned32 );
    axdrSetByte( stream, (value&0xff000000)>>24 );
    axdrSetByte( stream, (value&0xff0000)>>16 );
    axdrSetByte( stream, (value&0xff00)>>8 );
    axdrSetByte( stream, (value&0xff)>>0 );

    return true;
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

    return true;
}



//----------------------------------------------------------------------


bool axdrSetLength( AXDRStream* stream, uint32_t length )
{
    if(length < 0x80)
    {
        stream->data[0]  = (uint8_t)length;
        stream->data    += 1;
    }
    else if(length < 0xffff)
    {
        stream->data[0]  = 0x82;
        stream->data[1]  = (length>>8)&0x00ff;
        stream->data[2]  = length&0x00ff;
        stream->data    += 3;
    }
    else
    {
        stream->data[0]  = 0x84;
        stream->data[1]  = (length>>24)&0x000000ff;
        stream->data[2]  = (length>>16)&0x000000ff;
        stream->data[3]  = (length>>8)&0x000000ff;
        stream->data[4]  = length&0x000000ff;
        stream->data    += 5;
    }

    return true;
}


bool axdrGetLength( AXDRStream* stream, uint32_t* length )
{
    uint8_t     value   = stream->data[0];
    if( (value&0x80) == 0)
    {
        *length = value;
        stream->data    += 1;
    }
    else
    {
        uint8_t numberOfBytes   = value & 0x7f;
        if(numberOfBytes == 2)
        {
            *length = (stream->data[1]<<8) | (stream->data[2]);
            stream->data    += 3;
        }
        else if(numberOfBytes == 4)
        {
            *length = (stream->data[1]<<24) | (stream->data[2]<<16) | (stream->data[3]<<8) | (stream->data[4]);
            stream->data    += 5;
        }
        else
        {
            stream->data    += 1;
        }
    }

    return true;
}


bool axdrSetOctetString( AXDRStream* stream, uint8_t* data, uint32_t numberOfBytes)
{
    axdrSetByte(stream, DLMS_octet_string);
    axdrSetLength(stream, numberOfBytes);
    axdrSetBytes(stream, data,numberOfBytes);

    return true;
}


bool axdrSetStruct( AXDRStream* stream, uint32_t numberOfFields)
{
    axdrSetByte(stream, DLMS_structure);
    axdrSetLength(stream, numberOfFields);

    return true;
}


bool axdrSetArray( AXDRStream* stream, uint32_t numberOfElements)
{
    axdrSetByte(stream, DLMS_array);
    axdrSetLength(stream, numberOfElements);

    return true;
}



bool axdrGetArray( AXDRStream* stream, uint32_t* numberOfElements)
{
    axdrGetByteAndCheck( stream, NULL, DLMS_array );
    axdrGetLength( stream, numberOfElements );

    return true;
}

bool axdrGetStruct( AXDRStream* stream, uint32_t* numberOfFields)
{
    axdrGetByteAndCheck( stream, NULL, DLMS_structure );
    axdrGetLength( stream, numberOfFields );

    return true;
}

bool axdrGetOctetString( AXDRStream* stream, uint8_t* data, uint32_t dataMaxSize, uint32_t* numberOfBytes)
{
    axdrGetByteAndCheck( stream, NULL, DLMS_octet_string );

    axdrGetLength( stream, numberOfBytes );
    assert( *numberOfBytes <= dataMaxSize );

    axdrGetBytes( stream, data,*numberOfBytes );

    return true;
}



bool axdrSetDateTime( AXDRStream* stream, DLMSDateTime* datetime )
{
    axdrSetByte(stream, DLMS_octet_string);
    axdrSetByte(stream, 12);

    axdrSetByte(stream, (datetime->date.year&0xff00)>>8);
    axdrSetByte(stream, datetime->date.year&0xff);
    axdrSetByte(stream, datetime->date.month);
    axdrSetByte(stream, datetime->date.dayOfMonth);
    axdrSetByte(stream, datetime->date.dayOfWeek);
    axdrSetByte(stream, datetime->time.hour);
    axdrSetByte(stream, datetime->time.minute);
    axdrSetByte(stream, datetime->time.seconds);
    axdrSetByte(stream, datetime->time.hundredths);

    axdrSetByte(stream, (datetime->deviation&0xff00)>>8);
    axdrSetByte(stream, datetime->deviation&0xff);
    axdrSetByte(stream, datetime->status);
}

bool axdrSetDate( AXDRStream* stream, DLMSDate* date )
{
    axdrSetByte(stream, DLMS_octet_string);
    axdrSetByte(stream, 5);

    axdrSetByte(stream, (date->year&0xff00)>>8);
    axdrSetByte(stream, date->year&0xff);
    axdrSetByte(stream, date->month);
    axdrSetByte(stream, date->dayOfMonth);
    axdrSetByte(stream, date->dayOfWeek);
}

bool axdrSetTime( AXDRStream* stream, DLMSTime* time )
{
    axdrSetByte(stream, DLMS_octet_string);
    axdrSetByte(stream, 4);

    axdrSetByte(stream, time->hour);
    axdrSetByte(stream, time->minute);
    axdrSetByte(stream, time->seconds);
    axdrSetByte(stream, time->hundredths);
}


bool axdrGetDateTime( AXDRStream* stream, DLMSDateTime* datetime )
{
    axdrGetByteAndCheck( stream, NULL, DLMS_octet_string );
    axdrGetByteAndCheck( stream, NULL, 12 );

    uint8_t     lo;
    uint8_t     hi;
    axdrGetByte( stream, &hi );
    axdrGetByte( stream, &lo );
    datetime->date.year  = (hi<<8) | lo;
    
    axdrGetByte( stream, &datetime->date.month );
    axdrGetByte( stream, &datetime->date.dayOfMonth );
    axdrGetByte( stream, &datetime->date.dayOfWeek );
}

bool axdrGetDate( AXDRStream* stream, DLMSDate* date )
{
    axdrGetByteAndCheck( stream, NULL, DLMS_octet_string );
    axdrGetByteAndCheck( stream, NULL, 5 );

    uint8_t     lo;
    uint8_t     hi;
    axdrGetByte( stream, &hi );
    axdrGetByte( stream, &lo );
    date->year  = (hi<<8) | lo;
    
    axdrGetByte( stream, &date->month );
    axdrGetByte( stream, &date->dayOfMonth );
    axdrGetByte( stream, &date->dayOfWeek );
}

bool axdrGetTime( AXDRStream* stream, DLMSTime* time )
{
    axdrGetByteAndCheck( stream, NULL, DLMS_octet_string );
    axdrGetByteAndCheck( stream, NULL, 4 );

    axdrGetByte( stream, &time->hour );
    axdrGetByte( stream, &time->minute );
    axdrGetByte( stream, &time->seconds );
    axdrGetByte( stream, &time->hundredths );
}



//----------------------------------------------------------------------
// Test app.

int main()
{
    AXDRStream  stream;
    uint8_t     data[]  = {0x7e, 0x00, 0x7e};

    axdrStreamFromBytes( &stream, &data[0], sizeof(data) );
}

