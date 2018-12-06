
#include "Hash.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdlib.h>
#include "crc32.h"


struct
{
    hash_t      hash;
    uint32_t    numberOfBytes;
    uint8_t     value[MAX_VALUE_SIZE];

} hashCache[HASH_HISTORY_SIZE]  = {0};

hash_t  hashHistory[HASH_HISTORY_SIZE]  = {0};

//
hash_t generateHash( uint8_t* data, uint32_t numberOfBytes )
{
    hash_t  hash    = 0;
    crc32( data, numberOfBytes, 0x00000000, &hash );
    hash    &= ((uint32_t)(1<<BITS_PER_HASH))-1;
    return hash;
}


//
bool isHashInHistory( hash_t hash )
{
    for(uint32_t i=0; i<sizeof(hashHistory)/sizeof(hashHistory[0]); i++)
    {
        if( hashHistory[i] == hash )
        {
            return true;
        }   
    }   
    
    return false;
}   

//
void addHashToHistory( hash_t hash )
{
    memmove( &hashHistory[0], &hashHistory[1], sizeof(hashHistory)-sizeof(hashHistory[0]) );
    hashHistory[ sizeof(hashHistory)/sizeof(hashHistory[0])-1 ]   = hash;
}   




//
void addHashAndValueToHistory( uint8_t* value, uint32_t numberOfBytes )
{
    hash_t  hash    = generateHash( value, numberOfBytes );

    memmove( &hashCache[0], &hashCache[1], sizeof(hashCache)-sizeof(hashCache[0]) );

    hashCache[ sizeof(hashCache)/sizeof(hashCache[0])-1 ].hash          = hash;
    hashCache[ sizeof(hashCache)/sizeof(hashCache[0])-1 ].numberOfBytes = numberOfBytes;
    memcpy( &hashCache[ sizeof(hashCache)/sizeof(hashCache[0])-1 ].value[0], value, numberOfBytes );

    //printf("- adding %d bytes for hash %08x-\n",numberOfBytes,hash);
}

//
void getValueForHash( hash_t hash, uint8_t* data, uint32_t* numberOfBytes )
{
    //printf("-Wanting value for hash %08x-\n",hash);
    *numberOfBytes  = 0;
    for(uint32_t i=0; i<sizeof(hashCache)/sizeof(hashCache[0]); i++) 
    {
        if( hashCache[i].hash == hash )
        {
            *numberOfBytes  = hashCache[i].numberOfBytes;
            memcpy( data, &hashCache[i].value[0], *numberOfBytes );
            break;
        }
    }
}


