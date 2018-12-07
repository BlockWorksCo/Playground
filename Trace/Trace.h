//
// uint32 encoded using a continuation-bit, 0 indicates another byte to come, 1 indicates no more
// bytes.
// This scheme means there is an advantage gained while storing values closer to zero as less bytes
// need to be stored to represent the data.
// If the numeric value is combined with a set of flags, the flags should be in the LSB, meaning they
// do not create an artificially large-valued number.
//
// Timestamps:
// up to uint32-max in total, Type in high part of timestamp.
// First uint32 in packet is a time-base. All other timestamps are deltas from this. 
// Resolution is 0.1s
// time can use lower 2048 (204.8sec, 11 bits), leaving upper 4 bits for type and 1 for continuation-bit.
// 0123456701234567
// 0TTTTttttttttttt
//
// Type 0: marker
// <markerAndType:uint32>
// 01234567
// 1xxxxxtt   x = marker-value, t = type.
// typical size 1b
//
// Type 1: hex dump:
// <numberOfBytesinBLOB&Type> <BLOB>
// 01234567
// 1nnnnntt     n = number-of-bytes, t = type
// 1 byte can encode up to32 bytes of data.
// typical size 1 + n 
//
// Type 2: printf
// <formatAddressAndFlags:uint32> <:uint32> <:uint32> ...
// 012345670123456701234567
// 0aaaaaaa0aaaaaaa1aaaaatt     a=address t = type.
// 3 bytes can encode any string address in a 512KB image (19-bits)
// parameters will follow as specified by the format-string.
//
// Type 3: Timestamp
//
// In typical 256b packet:
// 256/1 = 256 type 0.
// 256/(1+31) = 8 type 1.
// 256/3 = 85 type 1 with no params.
// 256/(3+2) = 51 type 1 with 2 small params.
//


#ifndef __TRACE_H__
#define __TRACE_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <inttypes.h>
#include "crc32.h"



//
typedef enum
{
    Marker          = 0,
    HexDump         = 1,
    PrintF          = 2,
    CachedBLOB      = 3,
    TruncatedBLOB   = 4,

} TraceEntryType;

#define BITS_PER_TYPE               (3)
#define TYPE_MASK                   (7)


#define BITS_PER_HASH                   (24)



void traceEncodeUInt8( uint8_t value, uint8_t** ptr, bool lastFlag );
void traceEncodeUInt32( uint32_t value, uint8_t** ptr );
void traceEncodeZeroTerminatedBLOB( uint8_t* blob, uint32_t numberOfBytes, uint8_t** ptr );
void traceEncodeFixedSizeBLOB( uint8_t* blob, uint32_t numberOfBytes, uint8_t** ptr );
void traceEncodeBLOB( uint8_t* blob, uint32_t numberOfBytes, uint8_t** ptr );
void traceEncodeHex( uint8_t* data, uint32_t numberOfBytes, uint8_t** ptr );
void traceEncodeTruncatedHex( uint8_t* data, uint32_t numberOfBytes, uint8_t** ptr );
void traceEncodeMarker( uint32_t marker, uint8_t** ptr );
uint32_t encodeConstantStringPointer( const char* text );
void traceUseImageFile( char* fileName, uint32_t baseAddressOfImage );
void traceDecode( uint8_t** ptr );
void traceEncodePrintfVariadic( uint8_t** ptr, const char* format, va_list args );
void traceEncodePrintf( uint8_t** ptr, const char* format, ... );

void tracePacketFlush();


//
void traceOutput( const char* text );
void traceTransmitPacket( uint8_t* packet, uint32_t numberOfBytes );
uint32_t traceGetTime();


#endif


