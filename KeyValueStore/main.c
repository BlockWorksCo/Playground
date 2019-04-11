


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


#define NUMBER_OF_ELEMENTS(a)   (sizeof(a)/sizeof(a[0]))


// Chosen because of FLASH erase-value of 0xff.
#define INVALID_SEQUENCE_NUMBER (0xffffffff)

#define INVALID_INDEX           (0xffffffff)


//
typedef struct
{
    uint32_t    sequenceNumber;
    uint32_t    key;
    uint32_t    value;

} KeyValueStoreElement;


//
KeyValueStoreElement    kvsBuffer[128]  = {0};


//
uint32_t kvsLatestIndex()
{
    uint32_t    latestSequenceNumber    = INVALID_SEQUENCE_NUMBER;
    uint32_t    latestIndex             = INVALID_INDEX;

    for( uint32_t i=0; i<NUMBER_OF_ELEMENTS(kvsBuffer); i++ ) {

        if( kvsBuffer[i].sequenceNumber != INVALID_SEQUENCE_NUMBER ) {
        
            // This is a valid element.

            if( (kvsBuffer[i].sequenceNumber >= latestSequenceNumber) || (latestIndex == INVALID_INDEX) ) {

                // EITHER this sequence-number is greater than all previous 
                // OR this is the first valid element that we've come across.

                latestSequenceNumber    = kvsBuffer[i].sequenceNumber;
                latestIndex             = i;
            }        

        }
    }

    return latestIndex;
}


//
bool kvsSet( uint32_t key, uint32_t value )
{
    uint32_t    latestIndex         = kvsLatestIndex();
    uint32_t    insertionIndex      = 0;
    uint32_t    nextSequenceNumber  = 0;

    // Determine where to put this new kv-pair.
    if( latestIndex == INVALID_SEQUENCE_NUMBER ) {
        // no keys in buffer, so start at the beginning.
        insertionIndex      = 0;
        nextSequenceNumber  = 0;
    }
    else {
        // some keys in buffer, so lets append.
        insertionIndex      = (latestIndex + 1) % NUMBER_OF_ELEMENTS(kvsBuffer);
        nextSequenceNumber  = kvsBuffer[latestIndex].sequenceNumber + 1;
    }
    
    // Insert into the dataset.
    kvsBuffer[insertionIndex].sequenceNumber    = nextSequenceNumber;
    kvsBuffer[insertionIndex].key               = key;
    kvsBuffer[insertionIndex].value             = value;

    return true;
}


//
bool kvsGet( uint32_t key, uint32_t* value )
{
    uint32_t    latestIndex     = kvsLatestIndex();

    // Search backwards through the entire buffer for the specified key
    // then return the value.
    for( uint32_t i=0; i<NUMBER_OF_ELEMENTS(kvsBuffer); i++ ) {
        
        uint32_t    indexToCheck    = (latestIndex - i) % NUMBER_OF_ELEMENTS(kvsBuffer);
        if( (kvsBuffer[indexToCheck].key == key) && (kvsBuffer[indexToCheck].sequenceNumber != INVALID_SEQUENCE_NUMBER) ) {

            // We've found the key, return the value.
            *value  = kvsBuffer[indexToCheck].value;
            return true;
        }
    }

    return false;
}


// Set the entire buffer to the erased-value.
void kvsClear()
{
    memset( &kvsBuffer[0], 0xff, sizeof(kvsBuffer) );
}


//
int main()
{
    kvsClear();

    kvsSet( 0x1234, 0xabcd );
    kvsSet( 0x1235, 0xdcba );
    kvsSet( 0x1236, 0x0123 );
    kvsSet( 0x1237, 0x3210 );

    uint32_t    value   = 0;

    kvsGet( 0x1234, &value );
    printf("%x = %x\n",0x1234,value);

    kvsGet( 0x1235, &value );
    printf("%x = %x\n",0x1235,value);

    kvsGet( 0x1236, &value );
    printf("%x = %x\n",0x1236,value);

    kvsGet( 0x1237, &value );
    printf("%x = %x\n",0x1237,value);
}


