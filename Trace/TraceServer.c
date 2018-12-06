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
#include <stdlib.h>


uint32_t    totalSize       = 0;




// Output the text.
void traceOutput( const char* text )
{
    static uint32_t lineCount   = 0;
    printf( "[%08x] %s\n", lineCount, text );
    lineCount++;

    totalSize   += strlen(text);
}


// Entry point.
int main( int argc, char* argv[] )
{
    uint8_t     tracePacket[256];

    // Work out the base address of the imagefile we're tracing.
    uintptr_t rodataBase  = atoi( argv[2] );
    printf("rodataBase = %"PRIiPTR"\n", rodataBase);

    // Inform the library about the imagefile we're tracing.
    traceUseImageFile( argv[1], rodataBase );

    // Read packet from stdin.
    uint32_t    serialisedSize  = 0;
    while( feof(stdin) == 0 ) 
    {
        uint8_t     hi  = fgetc(stdin);
        uint8_t     lo  = fgetc(stdin);
        hi  = (hi <= '9' ? hi-'0' : (hi-'a')+10 );
        lo  = (lo <= '9' ? lo-'0' : (lo-'a')+10 );
        uint8_t     value   = (hi<<4) | lo;

        tracePacket[serialisedSize] = value;
        serialisedSize++;
    }

    // Decode
    traceDecodePacket( &tracePacket[0] );

    // Show some stats.
    printf("\nSummary:\n");
    printf("serialised size = %"PRIu32"\n", serialisedSize);
    printf("deserialised size = %d\n", totalSize);
    printf("serialise size = %.1f%%\n",(100.0/totalSize)*(float)serialisedSize);
}


