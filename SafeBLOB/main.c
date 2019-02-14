


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>


#define MAGIC_MARKER    ((uint16_t)~0xDEAD)   // not-dead ;o)


typedef struct
{
    uint16_t    magic;
    uint32_t    start;
    uint32_t    end;
    uint32_t    crc;
    uint32_t    sequenceNumber;

    uint8_t     data[1];    // final field;

} Span;



uint8_t rawStorage[1024*16];


uint32_t crcOfBytes( uint8_t* data, uint32_t numberOfBytes )
{
    uint32_t    total   = 0x12345;
    
    for(uint32_t i=0; i<numberOfBytes; i++) {
        total   += data[i];
    }

    return total;
}



void showSpans( uint8_t* data, uint32_t numberOfBytes )
{
    Span*       span            = (Span*)data;
    uint32_t    sequenceNumber  = (uint32_t)0;

    do
    {
        Span*       nextSpan    = NULL;
        uint32_t    thisCRC     = crcOfBytes( &span->data[0], span->end - span->start );

        if( thisCRC == span->crc ) {

            // good span... Move past it.
            printf("%08x: %d->%d\n", span->sequenceNumber, span->start, span->end);
            nextSpan    = (Span*)(((uint8_t*)span) + sizeof(Span) + (span->end - span->start));
        }
        else {

            // bad span... do a span-scan.
            do
            {
                uint8_t*    temp    = (uint8_t*)span;
                nextSpan    = (Span*)(temp+1);
                span        = nextSpan;

            } while( (nextSpan->magic != MAGIC_MARKER) && ((uintptr_t)span < (uintptr_t)(data+numberOfBytes)) );
        }

        // Move to next. 
        span    = nextSpan;

    } while( (span != NULL) && ((uintptr_t)span < (uintptr_t)(data+numberOfBytes)) );
}



void addSpan( uint8_t* rawStorage, uint32_t rawStorageSize,  uint32_t start, uint8_t* data, uint32_t numberOfBytes )
{
    Span*   span    = (Span*)&rawStorage[0];

    span->magic = MAGIC_MARKER;
    span->start = start;
    span->end   = start + numberOfBytes;
    span->sequenceNumber    = 123;
    span->crc               = crcOfBytes( data, numberOfBytes );
    memcpy( &span->data[0], data,numberOfBytes );
}


int main()
{
    addSpan( &rawStorage[0], sizeof(rawStorage) ,  0, "Hello World",  12 );
    showSpans( &rawStorage[0], sizeof(rawStorage)  );
}

