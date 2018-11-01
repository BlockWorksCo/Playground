


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>



//
uint8_t     tempData[1024];
uint8_t*    tracePacket     = &tempData[0];
uint8_t*    tracePacketPtr  = NULL;
uint32_t    lastTraceTimestamp   = 0;


//
uint32_t traceGetTimestamp()
{
    return 123456;
}

//
void traceOutputPacket()
{
    // Output the packet.
    printf("Output packet.\n");

    // Reset packet.
    tracePacket = &tempData[0];
}

//
void traceEncodeUInt32( uint32_t value, uint8_t** ptr )
{
    memcpy( (void*)*ptr, (void*)&value, sizeof(uint32_t) );
    *ptr    += sizeof(uint32_t);
}

//
void traceOutput( uint32_t marker )
{
    uint32_t    timestamp   = traceGetTimestamp();

    //
    traceEncodeUInt32( timestamp, &tracePacketPtr ); 
    traceEncodeUInt32( marker, &tracePacketPtr ); 

    //
    traceOutputPacket();

    //
    lastTraceTimestamp  = timestamp;
}

//
int main()
{
    tracePacket = &tempData[0];
    tracePacketPtr  = tracePacket;

    traceOutput( 123 );
    traceOutput( 456 );
}


