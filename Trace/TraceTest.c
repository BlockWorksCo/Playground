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



#include "Trace.h"
#include "TracePacket.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <inttypes.h>
#include "crc32.h"




// Packet & stream data.
uint8_t     tracePacket[256];
uint8_t*    tracePacketPtr  = NULL;


//
void traceTransmitPacket( uint8_t* packet, uint32_t numberOfBytes )
{
}


//
int main()
{
    // Encode
    tracePacketPtr  = &tracePacket[0];

    traceEncodeMarker( 1, &tracePacketPtr );
    traceEncodeMarker( 2, &tracePacketPtr );
    traceEncodeMarker( 3, &tracePacketPtr );
    traceEncodePrintf( &tracePacketPtr, "Hello World." );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%d)", 123 );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%d, %d)", 456,789 );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%x, %x, %x)", 0xab,0xabcd, 0x0123abcd );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%c, %f, %g)", 'A',3.14, 304.0 );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%s)", "Blaa!" );
    traceEncodePrintf( &tracePacketPtr, "This is a long message to see if it improves the compression ratio....");
    uint8_t data[]  = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x12,0x34,0x45,0x67,0x89,0xab,0xcd,0xef};
    traceEncodeHex( &data[0], sizeof(data), &tracePacketPtr );
    traceEncodeHex( &data[0], sizeof(data), &tracePacketPtr );
    traceEncodeHex( &data[0], sizeof(data), &tracePacketPtr );
    traceEncodeHex( &data[0], sizeof(data), &tracePacketPtr );
    traceEncodeHex( &data[0], sizeof(data), &tracePacketPtr );
    uint8_t bigBLOB[]  = {0x12,0x34,0x45,0x67,0x89,0xab,0xcd,0xef, 0x01,0x35,0x46,0x10,0x19,0x23};
    traceEncodeTruncatedHex( &bigBLOB[0], sizeof(bigBLOB), &tracePacketPtr );
    uint8_t ipv6Addr[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    uint32_t    ipv4Addr    = 0xc0108123;
    traceEncodePrintf( &tracePacketPtr, "IPv6. (%p6) IPv4 [%p4]", ipv6Addr, &ipv4Addr );
    uint8_t iid[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    traceEncodePrintf( &tracePacketPtr, "IID. (%pi)", iid );
    uint8_t eui64[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    traceEncodePrintf( &tracePacketPtr, "EUI 64. (%pm)", eui64 );
    uint8_t eui48[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    traceEncodePrintf( &tracePacketPtr, "EUI 48. (%pM)", eui48 );
    uint8_t obis[6]  = {0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    traceEncodePrintf( &tracePacketPtr, "OBIS Code. (%po)", obis );

    ptrdiff_t   serialisedSize    = tracePacketPtr - &tracePacket[0];
    //printf("\n[");
    for(uint32_t i=0; i<serialisedSize; i++) 
    {
        printf("%02x",tracePacket[i]);
    }
    //printf("]\n");
}


