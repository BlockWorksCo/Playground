


#include "axdr.h"




void axdrSetLength(Stream* stream, uint32_t length)
{
    uint8_t*    streamBytes = (uint8_t*)*stream;
    
    if(length < 0x80)
    {
        streamBytes[0]  = (uint8_t)length;
        *stream      = (Stream)(streamBytes+1);
    }
    else if(length < 0xffff)
    {
        streamBytes[0]  = 0x82;
        streamBytes[1]  = (length>>8)&0x00ff;
        streamBytes[2]  = length&0x00ff;
        *stream      = (Stream)(streamBytes+3);
    }
    else
    {
        streamBytes[0]  = 0x84;
        streamBytes[1]  = (length>>24)&0x000000ff;
        streamBytes[2]  = (length>>16)&0x000000ff;
        streamBytes[3]  = (length>>8)&0x000000ff;
        streamBytes[4]  = length&0x000000ff;
        *stream      = (Stream)(streamBytes+5);
    }
}

void axdrSetOctetString(Stream* stream, uint8_t* data, uint32_t numberOfBytes)
{
    streamSetUint8(stream, octet_string);
    axdrSetLength(stream, numberOfBytes);
    streamSetUint8Array(stream, data,numberOfBytes);
}


void axdrSetUint32(Stream* stream, uint32_t value)
{
    streamSetUint8(stream, unsigned32);
    streamSetUint8Array(stream, (void*)&value,sizeof(value) );
}


void axdrSetUint16(Stream* stream, uint16_t value)
{
    streamSetUint8(stream, unsigned16);
    streamSetUint8Array(stream, (void*)&value,sizeof(value) );
}


void axdrSetUint8(Stream* stream, uint8_t value)
{
    streamSetUint8(stream, unsigned8);
    streamSetUint8Array(stream, (void*)&value,sizeof(value) );
}


void axdrSetStruct(Stream* stream, uint32_t numberOfFields)
{
    streamSetUint8(stream, structure);
    axdrSetLength(stream, numberOfFields);
}


void axdrSetArray(Stream* stream, uint32_t numberOfElements)
{
    streamSetUint8(stream, array);
    axdrSetLength(stream, numberOfElements);
}





void axdrGetLength(Stream* stream, uint32_t* length)
{
    uint8_t*    streamBytes = (uint8_t*)*stream;
    
    uint8_t     value   = streamBytes[0];
    if( (value&0x80) == 0)
    {
        *length = value;
        *stream  = (Stream)(streamBytes+1);
    }
    else
    {
        uint8_t numberOfBytes   = value & 0x7f;
        if(numberOfBytes == 2)
        {
            *length = (streamBytes[1]<<8) | (streamBytes[2]);
            *stream  = (Stream)(streamBytes+3);
        }
        else if(numberOfBytes == 4)
        {
            *length = (streamBytes[1]<<24) | (streamBytes[2]<<16) | (streamBytes[3]<<8) | (streamBytes[4]);
            *stream  = (Stream)(streamBytes+5);
        }
        else
        {
            *stream  = (Stream)(streamBytes+1);
        }
    }
}

void axdrGetArray(Stream* stream, uint32_t* numberOfElements)
{
    uint8_t    tag   = 0;
    streamGetUint8( stream, &tag );
    assert(tag == array);

    axdrGetLength( stream, numberOfElements );
}

void axdrGetStruct(Stream* stream, uint32_t* numberOfFields)
{
    uint8_t    tag   = 0;
    streamGetUint8( stream, &tag );
    assert(tag == structure);

    axdrGetLength( stream, numberOfFields );
}


void axdrGetOctetString(Stream* stream, uint8_t* data, uint32_t dataMaxSize, uint32_t* numberOfBytes)
{
    uint8_t    tag   = 0;
    streamGetUint8( stream, &tag );
    assert(tag == octet_string);

    axdrGetLength( stream, numberOfBytes );
    assert( *numberOfBytes < dataMaxSize );

    streamGetUint8Array( stream, data,*numberOfBytes );
}

void axdrGetUint32(Stream* stream, uint32_t* value)
{
    uint8_t    tag   = 0;
    streamGetUint8( stream, &tag );
    assert(tag == unsigned32);

    streamGetUint8Array( stream, (void*)value,sizeof(*value) );
}


void axdrGetUint16(Stream* stream, uint16_t* value)
{
    uint8_t    tag   = 0;
    streamGetUint8( stream, &tag );
    assert(tag == unsigned16);

    streamGetUint8Array( stream, (void*)value,sizeof(*value) );
}


void axdrGetUint8(Stream* stream, uint8_t* value)
{
    uint8_t    tag   = 0;
    streamGetUint8( stream, &tag );
    assert(tag == unsigned8);

    streamGetUint8Array( stream, (void*)value,sizeof(*value) );
}


