
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

} hashHistory[HASH_HISTORY_SIZE]  = {0};


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
    return false;
}

//
void addHashAndValueToHistory( uint8_t* value, uint32_t numberOfBytes )
{
    hash_t  hash    = generateHash( value, numberOfBytes );

    memmove( &hashHistory[0], &hashHistory[1], sizeof(hashHistory)-sizeof(hashHistory[0]) );

    hashHistory[ sizeof(hashHistory)/sizeof(hashHistory[0])-1 ].hash          = hash;
    hashHistory[ sizeof(hashHistory)/sizeof(hashHistory[0])-1 ].numberOfBytes = numberOfBytes;
    memcpy( &hashHistory[ sizeof(hashHistory)/sizeof(hashHistory[0])-1 ].value[0], value, numberOfBytes );

    //printf("- adding %d bytes for hash %08x-\n",numberOfBytes,hash);
}

//
void getValueForHash( hash_t hash, uint8_t* data, uint32_t* numberOfBytes )
{
    //printf("-Wanting value for hash %08x-\n",hash);
    *numberOfBytes  = 0;
    for(uint32_t i=0; i<sizeof(hashHistory)/sizeof(hashHistory[0]); i++) 
    {
        if( hashHistory[i].hash == hash )
        {
            *numberOfBytes  = hashHistory[i].numberOfBytes;
            memcpy( data, &hashHistory[i].value[0], *numberOfBytes );
            break;
        }
    }
}


