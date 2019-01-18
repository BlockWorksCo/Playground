

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


/////////////////////////////////////////////////////////////////////

#define FLASH_FILENAME      ("flash.bin")
#define FLASH_SECTOR_SIZE   (128)


void flashEraseSector( uint32_t sectorId )
{
    FILE*   pFile   = NULL;

    pFile   = fopen( FLASH_FILENAME, "rwb+" );
    if( pFile != NULL )
    {
        uint8_t temp[FLASH_SECTOR_SIZE];
        memset( &temp[0], 0xff, sizeof(temp) );
        fseek( pFile, FLASH_SECTOR_SIZE*sectorId, SEEK_SET );
        printf("<pos=%ld\n",ftell(pFile));
        fwrite( &temp[0], 1, sizeof(temp), pFile );
        fclose(pFile);
    }
}


void flashWriteSector( uint32_t sectorId, uint32_t offset, uint8_t* bytes, uint32_t numberOfBytes )
{
    FILE*   pFile   = NULL;

    pFile   = fopen( FLASH_FILENAME, "rwb+" );
    if( pFile != NULL )
    {
        fseek( pFile, (FLASH_SECTOR_SIZE*sectorId)+offset, SEEK_SET );
        printf("<pos=%ld\n",ftell(pFile));
        size_t  bytesWritten    = fwrite( &bytes[0], 1,numberOfBytes, pFile );
        printf("<written=%zd\n",bytesWritten);
        fclose(pFile);
    }
}


void flashReadSector( uint32_t sectorId, uint32_t offset, uint8_t* bytes, uint32_t numberOfBytes )
{
    FILE*   pFile   = NULL;

    pFile   = fopen( FLASH_FILENAME, "rwb+" );
    if( pFile != NULL )
    {
        fseek( pFile, (FLASH_SECTOR_SIZE*sectorId)+offset, SEEK_SET );
        printf("<pos=%ld\n",ftell(pFile));
        size_t  bytesRead   = fread( &bytes[0], 1,numberOfBytes, pFile );
        printf("<read=%zd\n",bytesRead);
        fclose(pFile);
    }
}


/////////////////////////////////////////////////////////////////////
//
//  |0................................................ ...n|
//             |10...........................40|
//       |5...........15|
//                 |12.............30|
//
// When reading spans, read from latest-change, backwards identifying 
// portions of the span you need.
//
// When writing spans, just append to the buffer with metadata incorporating
// a header of (spanStart,spanEnd,sequenceNumber,crc).
// SequenceNumber is an increasing number to identify the most recent
// position in the chain.
//
// Before there is less space left in the entire storage-area than is
// required to store the entire BLOB, rewrite the entire BLOB as one
// span and continue around the storage-area treating it as a 
// circular-buffer.
//
// When writing a single-span, never cross a sector-boundary as this would
// then require two FLASH writes to complete.
//
// Maintaining a strict single-write-per-span requirement means the holdup
// time for power-failure is less than the time to write a single sector.
// The write of the crc as the last part of the span makes the operation
// atomic. Sectors are considered to be pre-erased, which can be done
// safely without regard to power-fail. Erase-check can be performed
//
// On startup, the end position is identified by scanning and crc cheking
// the individual spans. The largest sequence number is the latest.
//
// Wear-levelling is obtained by over-allocating space for the LSB to
// work within.
//
// Bad-block management occurs by the non-critical erase-check that
// will identify erase-failures.
//
// This provides a version-controlled BLOB with automatic roll-back.
//


// Metadata post-pended to a span.
typedef struct
{
    uint32_t    start;
    uint32_t    end;
    uint32_t    sequenceNumber;
    uint32_t    crc;

} SpanHeader;


static void readHeaderOfSpan( uint32_t offset, SpanHeader* spanHeader )
{
    memset( &spanHeader, 0x00, sizeof(*spanHeader) );
}


static uint32_t sequenceNumberOfSpan( uint32_t offset )
{
    SpanHeader  spanHeader  = {0};
    readHeaderOfSpan( offset, &spanHeader );
    return spanHeader.sequenceNumber;
}


static uint32_t lengthOfSpan( uint32_t offset )
{
    SpanHeader  spanHeader  = {0};
    readHeaderOfSpan( offset, &spanHeader );
    return spanHeader.end - spanHeader.start;
}


static bool isGoodSpan( uint32_t offset )
{
    SpanHeader  spanHeader  = {0};
    readHeaderOfSpan( offset, &spanHeader );

    // check length is < MAX_SPAN_SIZE
    // check start is < MAX_SPAN_SIZE
    // check end is < MAX_SPAN_SIZE
    // check CRC.
    return true;
}


// end position is the address of the byte immediately
// following the last (highest sequence numbered) span.
static uint32_t findSequenceEndPosition()
{
    uint32_t    offset          = 0;
    uint32_t    sequenceNumber  = 0;

    while( isGoodSpan( offset ) == true )
    {
        uint32_t    thisSequenceNumber  = sequenceNumberOfSpan( offset );
        if( thisSequenceNumber > sequenceNumber )
        {
            // This is within (not the end of) the chain.
            sequenceNumber  = thisSequenceNumber;
            offset  += lengthOfSpan( offset );
        }
        else
        {
            // End found.
            break;
        }
    }

    //
    return offset;
}


void lsbWriteSpan( uint32_t offset, uint8_t* bytes, uint32_t numberOfBytes )
{
    // scan forward to find end of span-chain.

    // write data to new span area.

    // write header to new span area.
}


void lsbReadSpan( uint32_t offset, uint8_t* bytes, uint32_t numberOfBytes )
{
}



/////////////////////////////////////////////////////////////////////

int main()
{
    flashEraseSector( 10 );

    uint8_t     stuff1[]   = "Hello World!";
    flashWriteSector( 10, 10, &stuff1[0], sizeof(stuff1) );

    uint8_t     stuff2[1024]    = {0};
    flashReadSector( 10, 10, &stuff2[0], sizeof(stuff1) );

    printf("[%s]\n", stuff2);
}


