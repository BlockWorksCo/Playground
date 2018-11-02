


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// The larger this value, the less data will be taken with timing info.
#define TRACE_TIMESTAMP_RESOLUTION      (10)

// Masks for working out the 7-bit portions.
#define MASK_B0                         (0x0000007f)
#define MASK_B1                         (0x00003f80)
#define MASK_B2                         (0x001fc000)
#define MASK_B3                         (0x0fe00000)
#define SHIFT_B0                        (0)
#define SHIFT_B1                        (7)
#define SHIFT_B2                        (14)
#define SHIFT_B3                        (21)

// Packet & stream data.
uint8_t     tempData[1024];
uint8_t*    tracePacket     = &tempData[0];
uint8_t*    tracePacketPtr  = NULL;

// memento of the timestamp for working out the timed deltas.
uint32_t    lastTraceTimestamp   = 0;


//
uint32_t traceGetTimestamp()
{
    return (uint32_t)clock();
}

//
void traceOutputPacket()
{
    // Output the packet.
    printf("Output packet.\n");

    // Reset packet.
    //tracePacket = &tempData[0];
}

//
void traceDecodeUInt8( uint8_t** ptr, uint8_t* value, bool* lastFlag )
{
    // Copy the values out of the stream.
    *value      = **ptr;
    *lastFlag   = false;

    printf("[%02x]",*value);

    // If the top bit is set, this is the last byte in the value.
    if( ((*value)&0x80) != 0 )
    {
        *lastFlag   =  true;
    }

    // Clear the top bit as that carries a control value, not data.
    *value  = *value & 0x7f;

    // Move the stream pointer along.
    *ptr = (*ptr) + 1;
}

//
void traceEncodeUInt8( uint8_t value, uint8_t** ptr, bool lastFlag )
{
    // Set the top bit if its the last in the sequence, clear it otherwise.
    value   &= 0x7f;
    if( lastFlag == true )
    {
        value   |= 0x80;
    }

    printf("<%02x>",value);

    // Insert the value into the byte stream.
    **ptr   = value;
    
    // Move the head-of-stream pointer along.
    *ptr    = (*ptr) + 1;
}

//
// Encode a 29-bit value.
//
void traceEncodeUInt32( uint32_t value, uint8_t** ptr )
{
    if( value < 0x7f )
    {
        // 7-bit case.
        traceEncodeUInt8( (value & MASK_B0) >> SHIFT_B0, ptr, true );
    }
    else if( value < 0x7fff )
    {
        // 14-bit case.
        traceEncodeUInt8( (value & MASK_B1) >> SHIFT_B1, ptr, false );
        traceEncodeUInt8( (value & MASK_B0) >> SHIFT_B0, ptr, true );
    }
    else if( value < 0x7fffff )
    {
        // 21-bit case.
        traceEncodeUInt8( (value & MASK_B2) >> SHIFT_B2, ptr, false );
        traceEncodeUInt8( (value & MASK_B1) >> SHIFT_B1, ptr, false );
        traceEncodeUInt8( (value & MASK_B0) >> SHIFT_B0, ptr, true );
    }
    else
    {
        // 28-bit case.
        traceEncodeUInt8( (value & MASK_B3) >> SHIFT_B3, ptr, false );
        traceEncodeUInt8( (value & MASK_B2) >> SHIFT_B2, ptr, false );
        traceEncodeUInt8( (value & MASK_B1) >> SHIFT_B1, ptr, false );
        traceEncodeUInt8( (value & MASK_B0) >> SHIFT_B0, ptr, true );
    }
}

//
void traceDecodeUInt32( uint32_t* value, uint8_t** ptr )
{
    bool        lastFlag    = false;
    
    // Must start off with no bits set.
    *value  = 0;

    // Repeatedly read 7 bits from the stream until we get to
    // a byte with the top bit set, then stop.
    do
    {
        // Shift up the previous value by 7 bits (we start off with 0 so
        // this is harmless initially).
        *value  = (*value) << 7;

        // Get the next byte out.
        uint8_t     byteValue   = 0;
        traceDecodeUInt8( ptr, &byteValue, &lastFlag );

        // Combine the new byte with the existing value.
        *value  = (*value) | byteValue;

    } while( lastFlag == false );
}


//
void traceOutput( uint32_t marker )
{
    // TODO: At start of packet, we need a full timestamp.

    // Work out the time delta since we were last called.
    uint32_t    timestamp       = traceGetTimestamp();
    uint32_t    deltaTimestamp  = (timestamp - lastTraceTimestamp) / TRACE_TIMESTAMP_RESOLUTION;

    printf("deltaTimestamp=%d\n",timestamp);

    // Put the tokens into the stream.
    traceEncodeUInt32( deltaTimestamp, &tracePacketPtr ); 
    traceEncodeUInt32( marker, &tracePacketPtr ); 

    // (Potentially) output the packet.
    traceOutputPacket();

    // Store the current timestamp so we can work out the delta for
    // next time.
    lastTraceTimestamp  = timestamp;
}

//
void traceInput( uint32_t* timeDelta, uint32_t* marker )
{
    // Parse the tokens from the stream.
    traceDecodeUInt32( timeDelta, &tracePacketPtr );
    traceDecodeUInt32( marker, &tracePacketPtr );

    // Multiply the timeDelta back up
    *timeDelta   *= TRACE_TIMESTAMP_RESOLUTION;
}

//
int main()
{
    tracePacket = &tempData[0];
    tracePacketPtr  = tracePacket;

    traceOutput( 0x1fff );
    traceOutput( 456 );
    traceOutput( 0xabcdef );


    tracePacket = &tempData[0];
    tracePacketPtr  = tracePacket;

    uint32_t    timeDelta   = 0;
    uint32_t    marker      = 0;
    traceInput( &timeDelta, &marker );
    printf(" +%d) %x\n", timeDelta, marker);

    traceInput( &timeDelta, &marker );
    printf(" +%d) %x\n", timeDelta, marker);

    traceInput( &timeDelta, &marker );
    printf(" +%d) %x\n", timeDelta, marker);
}


