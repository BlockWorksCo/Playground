


#include "axdr.h"



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


void axdrSetUint8__(AXDRStream* stream, uint8_t value)
{
    axdrSetUint8(stream, unsigned8);
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


void axdrGetUint8__(AXDRStream* stream, uint8_t* value)
{
    uint8_t    tag   = 0;
    axdrGetUint8( stream, &tag );
    assert(tag == unsigned8);

    axdrGetUint8Array( stream, (void*)value,sizeof(*value) );
}


