


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>


#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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

typedef struct
{
    Span*   latestSpan;
    uint8_t rawStorage[1024*16];

} LogStructuredBLOB;






uint32_t crcOfBytes( uint8_t* data, uint32_t numberOfBytes )
{
    uint32_t    total   = 0x12345;
    
    for(uint32_t i=0; i<numberOfBytes; i++) {
        total   += data[i];
    }

    return total;
}



void scanForLatest( LogStructuredBLOB* blob )
{
    Span*       span            = (Span*)&blob->rawStorage[0];
    void*       end             = &blob->rawStorage[sizeof(blob->rawStorage)];
    uint32_t    sequenceNumber  = (uint32_t)0;

    blob->latestSpan    = NULL;

    do
    {
        Span*       nextSpan    = NULL;
        uint32_t    thisCRC     = crcOfBytes( &span->data[0], span->end - span->start );

        if( thisCRC == span->crc ) {

            // good span... Move past it.
            if( span->sequenceNumber >= blob->latestSpan->sequenceNumber ) {
            
                printf("latestSpan @ %p\n",span);
                blob->latestSpan    = span;
            }

            nextSpan    = (Span*)(((uint8_t*)span) + sizeof(Span) + (span->end - span->start));
        }
        else {

            // bad span... do a span-scan.
            do
            {
                uint8_t*    temp    = (uint8_t*)span;
                nextSpan    = (Span*)(temp+1);
                span        = nextSpan;

            } while( (nextSpan->magic != MAGIC_MARKER) && ((uintptr_t)span < (uintptr_t)end) );
        }

        // Move to next. 
        span    = nextSpan;

    } while( (span != NULL) && ((uintptr_t)span < (uintptr_t)end) );
}

void showSpans( LogStructuredBLOB* blob )
{
    Span*       span            = (Span*)&blob->rawStorage[0];
    void*       end             = &blob->rawStorage[sizeof(blob->rawStorage)];
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

            } while( (nextSpan->magic != MAGIC_MARKER) && ((uintptr_t)span < (uintptr_t)end) );
        }

        // Move to next. 
        span    = nextSpan;

    } while( (span != NULL) && ((uintptr_t)span < (uintptr_t)end) );
}



void addSpan( LogStructuredBLOB* blob, uint32_t start, uint8_t* data, uint32_t numberOfBytes )
{
    // Find the position of the new span, if the address of the last byte 
    // in the span lies beyond the end of the storage, then wrap it around
    // to the beginning.
    Span*       span            = NULL;
    uint32_t    sequenceNumber  = 0;
    if(blob->latestSpan == NULL) {
        // No spans exist in the storage, start everything anew.
        span    = (Span*)&blob->rawStorage[0];
    }
    else { 
        // spans exist in the storage, put this span after the latest one and wrap iff needed.
        span    = (Span*)(((uint8_t*)blob->latestSpan) + sizeof(Span) + (blob->latestSpan->end - blob->latestSpan->start));
        void*   end     = &blob->rawStorage[sizeof(blob->rawStorage)];
        if( (uintptr_t)&span->data[numberOfBytes] >= (uintptr_t)end) {

            span    = (Span*)&blob->rawStorage[0];
        }

        sequenceNumber  = blob->latestSpan->sequenceNumber;
    }

    // Make the new span.
    span->magic             = MAGIC_MARKER;
    span->start             = start;
    span->end               = start + numberOfBytes;
    span->sequenceNumber    = sequenceNumber+1;
    span->crc               = crcOfBytes( data, numberOfBytes );
    memcpy( &span->data[0], data,numberOfBytes );

    // Set the end of the span-list to this new span.
    blob->latestSpan        = span;
}



// Walk backwards round the log, copying out the portions of spans that enter our desired range.
// We finish the walkback when the entire set of bytes has been read out.
void ReadBLOB( LogStructuredBLOB* blob, uint32_t start, uint32_t numberOfBytes, uint8_t* data, uint32_t maxNumberOfBytes )
{
    // Iterate over the desired range, finding spans that cover the starting 
    // position.
    for( uint32_t i=0; i<numberOfBytes; i++ ) {
        
        Span*   span    = blob->latestSpan;
        if( start+i >= span->start && start+i <= span->end ) {
            // This span contains the start point that we want.
            // lets find out where it ends.
            i   = MIN( span->end, start+numberOfBytes ) - start;
        }
    }
}


int main()
{
    LogStructuredBLOB   blob;
    scanForLatest( &blob );

    addSpan( &blob ,  0, "Hello World",  12 );
    addSpan( &blob ,  10, "Hello World",  12 );
    addSpan( &blob ,  20, "Hello World",  12 );

    uint8_t temp[64]    = {0};
    ReadBLOB( &blob, 0,32, &temp[0],sizeof(temp) );
    printf("[%s]\n", temp);

    showSpans( &blob );
}

