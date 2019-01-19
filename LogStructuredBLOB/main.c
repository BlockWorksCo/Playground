

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


uint32_t crc32_for_byte(uint32_t r) {
  for(int j = 0; j < 8; ++j)
    r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
  return r ^ (uint32_t)0xFF000000L;
}

void crc32(const void *data, size_t n_bytes, uint32_t* crc) {
  static uint32_t table[0x100];
  if(!*table)
    for(size_t i = 0; i < 0x100; ++i)
      table[i] = crc32_for_byte(i);
  for(size_t i = 0; i < n_bytes; ++i)
    *crc = table[(uint8_t)*crc ^ ((uint8_t*)data)[i]] ^ *crc >> 8;
}


//
//
//
uint32_t CRC32(uint32_t initialValue, uint8_t* data, uint32_t numberOfBytes)
{
    uint32_t    crc = initialValue;
    
    crc32( data, numberOfBytes, &crc );
    return crc;
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
// work within. All writes append to the circular buffer which eventually
// wraps around, spreading the wear over all the allocated space.
//
// Bad-block management occurs by the non-critical erase-check that
// will identify erase-failures.
// The entire storage space is scanned to find the end of the sequence
// thus any gaps are irrelevant.
//
// When searching for valid spans throughout the storage, we don't want to
// have to crc check everything from every byte position, so we place a
// 'magic' value in the header and scan for that before performing any
// further checks.
//
// This provides a version-controlled BLOB with automatic roll-back and
// deterministic shutdown behaviour in the presence of power-failures.
//

#define MAX_SPAN_SIZE       (128)
#define MAX_OFFSET          (MAX_SPAN_SIZE * 32)
#define SPAN_HEADER_MAGIC   (0xdeadbeef)

// Metadata post-pended to a span.
typedef struct
{
    uint32_t    magic;      // used for quick identification.
    uint32_t    start;
    uint32_t    end;
    uint32_t    sequenceNumber;
    uint32_t    crc;

} SpanHeader;



static uint32_t sectorIdFromStorageOffset( uint32_t blobOffset )
{
    return blobOffset / FLASH_SECTOR_SIZE;
}

static uint32_t sectorOffsetFromStorageOffset( uint32_t blobOffset )
{
    return blobOffset % FLASH_SECTOR_SIZE;
}

static void readHeaderOfSpan( uint32_t offset, SpanHeader* spanHeader )
{
    flashReadSector(    sectorIdFromStorageOffset(offset), 
                        sectorOffsetFromStorageOffset(offset), 
                        (uint8_t*)spanHeader, 
                        sizeof(*spanHeader) );
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

    // magic check.
    // Do this first as this is the cheap check.
    if( spanHeader.magic != SPAN_HEADER_MAGIC )
    {
        return false;
    }
    
    // check length is < MAX_SPAN_SIZE
    if( spanHeader.end-spanHeader.start > MAX_SPAN_SIZE )
    {
        return false;
    }

    // check CRC.
    // Do this last as this is the expensive check.
    uint8_t     spanData[MAX_SPAN_SIZE] = {0};
    flashReadSector(    sectorIdFromStorageOffset(offset+sizeof(spanHeader)), 
                        sectorOffsetFromStorageOffset(offset+sizeof(spanHeader)), 
                        &spanData[0], 
                        spanHeader.end-spanHeader.start);
    uint32_t    spanDataCRC = CRC32( 0, &spanData[0], spanHeader.end-spanHeader.start );
    
    if( spanHeader.crc != spanDataCRC )
    {
        // CRC did not match, so not a good span.
        return false;
    }

    return true;
}

//
static uint32_t findNextPossibleSpan( uint32_t offset )
{
    // Read a large buffer...
    uint8_t     spanData[MAX_SPAN_SIZE] = {0};
    flashReadSector(    sectorIdFromStorageOffset(offset), 
                        sectorOffsetFromStorageOffset(offset), 
                        &spanData[0], 
                        sizeof(spanData) );

    // ...then scan thru it to find the magic number.
    for(uint32_t thisOffset=offset; offset<MAX_OFFSET; offset++)
    {
        SpanHeader* spanHeader  = (SpanHeader*)&spanData[thisOffset];
        if( spanHeader->magic == SPAN_HEADER_MAGIC )
        {
            // We've found the offset of a magic-number, this is a 
            // potential span so lets return.
            return thisOffset;
        }
    }
    
    return (uint32_t)-1;
}


// end position is the address of the byte immediately
// following the last (highest sequence numbered) span.
static uint32_t findSequenceEndPosition( uint32_t* lastSequenceNumber )
{
    uint32_t    offset                          = 0;
    uint32_t    highestSequenceNumber           = 0;
    uint32_t    offsetOfHighestSequenceNumber   = 0;
    uint32_t    numberOfSpans                   = 0;

    // Find the offset of the span with the highest sequence number.
    do
    {
        offset  = findNextPossibleSpan( offset );
        if( offset < MAX_OFFSET )
        {
            // We have a potential span...
            if( isGoodSpan( offset ) == true )
            {
                // We have a good span, let check the sequence number
                uint32_t    thisSequenceNumber  = sequenceNumberOfSpan( offset );
                if( thisSequenceNumber >= highestSequenceNumber )
                {
                    // This is a new highest.
                    highestSequenceNumber           = thisSequenceNumber;
                    offsetOfHighestSequenceNumber   = offset;
                }

                // onwards...
                offset  += lengthOfSpan( offset ) + sizeof(SpanHeader);
                numberOfSpans++;
            }
            else
            {
                // this is a bad span, but it was a possible, so it must have
                // been a spurious magic value.
                // Lets skip over the magic value.
                offset  += sizeof(uint32_t);
            }
        }

    } while( offset < MAX_OFFSET );

    // Check for some corner cases.
    if( numberOfSpans == 0 )
    {
        // We haven't found any spans, the end position is at offset zero.
        *lastSequenceNumber   = 0;
        return 0;
    }
    else
    {
        // normal case... we have some spans and we found the end.

        // Return the last sequenceNumber.
        *lastSequenceNumber = highestSequenceNumber;

        // return the offset of the first free byte after the last span.
        return offsetOfHighestSequenceNumber + lengthOfSpan(offset);
    }
}


void lsbWriteSpan( uint32_t blobOffset, uint8_t* bytes, uint32_t numberOfBytes )
{
    // scan forward to find end of span-chain.
    uint32_t    lastSequenceNumber      = 0;
    uint32_t    endPositionInStorage    = findSequenceEndPosition( &lastSequenceNumber );

    printf("[endPosition = %d]\n",endPositionInStorage);

    // write data to new span area *first*.
    flashWriteSector(   sectorIdFromStorageOffset(endPositionInStorage+sizeof(SpanHeader)), 
                        sectorOffsetFromStorageOffset(endPositionInStorage+sizeof(SpanHeader)), 
                        bytes, 
                        numberOfBytes );

    // write header with the crc *after* the data has been written.
    SpanHeader  spanHeader  = 
    {
        .magic          = SPAN_HEADER_MAGIC,
        .start          = blobOffset,
        .end            = blobOffset+numberOfBytes,
        .sequenceNumber = lastSequenceNumber+1,
        .crc            = CRC32( 0, bytes, numberOfBytes ),
    };
    flashWriteSector(   sectorIdFromStorageOffset(endPositionInStorage), 
                        sectorOffsetFromStorageOffset(endPositionInStorage), 
                        (uint8_t*)&spanHeader, 
                        sizeof(spanHeader) );
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

    uint8_t     stuff2[MAX_SPAN_SIZE]    = {0};
    flashReadSector( 10, 10, &stuff2[0], sizeof(stuff1) );

    uint8_t     span1[MAX_SPAN_SIZE]     = {0};
    lsbWriteSpan( 0, &span1[0],sizeof(span1) );
    lsbWriteSpan( 1024, &span1[0],sizeof(span1) );

    printf("[%s]\n", stuff2);
}


