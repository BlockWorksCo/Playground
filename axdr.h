


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

#endif


