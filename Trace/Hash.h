
#ifndef __HASH_H__
#define __HASH_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <inttypes.h>
#include "Trace.h"
#include "crc32.h"



#define MAX_VALUE_SIZE      (128)
#define HASH_HISTORY_SIZE               (100)

//
typedef uint32_t    hash_t;

hash_t generateHash( uint8_t* data, uint32_t numberOfBytes );
bool isHashInHistory( hash_t hash );
void addHashToHistory( hash_t hash );
void getValueForHash( hash_t hash, uint8_t* data, uint32_t* numberOfBytes );
void addHashAndValueToHistory( uint8_t* value, uint32_t numberOfBytes );


#endif


