


#ifndef __AXDR_H__
#define __AXDR_H__


#include <stdbool.h>
#include <stdint.h>

//
//
//
typedef struct {

    uint8_t*    data;
    uint8_t*    dataEnd;

} AXDRStream;


bool axdrStreamFromBytes( AXDRStream* stream, uint8_t* data, uint8_t numberOfBytes );

bool axdrGetByte( AXDRStream* stream, uint8_t* byte );
bool axdrSetByte( AXDRStream* stream, uint8_t byte );
bool axdrGetByteAndCheck( AXDRStream* stream, uint8_t* byte, uint8_t expectedByte );
bool axdrSetBytes( AXDRStream* stream, uint8_t* bytes, uint32_t numberOfBytes );
bool axdrGetBytes( AXDRStream* stream, uint8_t* bytes, uint32_t numberOfBytes );

bool axdrSetUint8( AXDRStream* stream, uint8_t value ) ;
bool axdrGetUint8( AXDRStream* stream, uint8_t* value );
bool axdrSetUint16( AXDRStream* stream, uint16_t value );
bool axdrGetUint16( AXDRStream* stream, uint16_t* value );
bool axdrSetUint32( AXDRStream* stream, uint32_t value );
bool axdrGetUint32( AXDRStream* stream, uint32_t* value );

bool axdrSetLength( AXDRStream* stream, uint32_t length );
bool axdrGetLength( AXDRStream* stream, uint32_t* length );
bool axdrSetStruct( AXDRStream* stream, uint32_t numberOfFields);
bool axdrSetArray( AXDRStream* stream, uint32_t numberOfElements);
bool axdrGetArray( AXDRStream* stream, uint32_t* numberOfElements);
bool axdrGetStruct( AXDRStream* stream, uint32_t* numberOfFields);
bool axdrGetOctetString( AXDRStream* stream, uint8_t* data, uint32_t dataMaxSize, uint32_t* numberOfBytes);





#endif


