


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


#define NUMBER_OF_ELEMENTS(a)   (sizeof(a)/sizeof(a[0]))


// Chosen because of FLASH erase-value of 0xff.
#define INVALID_SEQUENCE_NUMBER (0x00000000)
#define ERASED_SEQUENCE_NUMBER  (0xffffffff)

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

        if( (kvsBuffer[i].sequenceNumber >= latestSequenceNumber) && (kvsBuffer[i].sequenceNumber != ERASED_SEQUENCE_NUMBER) ) {

            latestSequenceNumber    = kvsBuffer[i].sequenceNumber;
            latestIndex             = i;
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
    if( latestIndex == INVALID_INDEX ) {
        // no keys in buffer.
        insertionIndex      = 0;
        nextSequenceNumber  = 1;
    }
    else {
        // some keys in buffer.
        insertionIndex      = (latestIndex + 1) % NUMBER_OF_ELEMENTS(kvsBuffer);
        nextSequenceNumber  = kvsBuffer[latestIndex].sequenceNumber + 1;
    }
    
    kvsBuffer[insertionIndex].sequenceNumber    = nextSequenceNumber;
    kvsBuffer[insertionIndex].key               = key;
    kvsBuffer[insertionIndex].value             = value;

    return true;
}


//
bool kvsGet( uint32_t key, uint32_t* value )
{
    uint32_t    latestIndex     = kvsLatestIndex();

    for( uint32_t i=0; i<NUMBER_OF_ELEMENTS(kvsBuffer); i++ ) {
        
        uint32_t    indexToCheck    = (latestIndex - i) % NUMBER_OF_ELEMENTS(kvsBuffer);
        if( kvsBuffer[indexToCheck].key == key ) {
            return kvsBuffer[indexToCheck].value;
        }
    }

    return true;
}


void kvsClear()
{
    memset( &kvsBuffer[0], 0xff, sizeof(kvsBuffer) );
}


//
int main()
{
    kvsClear();

    kvsSet( 0x1234, 0xabcd );

    uint32_t    value   = 0;
    kvsGet( 0x1234, &value );

    printf("%x = %x\n",0x1234,value);
}


