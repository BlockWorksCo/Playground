


#ifndef __PERSISTENTQUEUE_H__
#define __PERSISTENTQUEUE_H__


#include <stdint.h>
#include <stdlib.h>

#include "ipv6.h"




void pqGet( uint32_t pqId, uint8_t* packet, size_t* maxNumberOfBytes );

void pqPut( uint32_t pqId, uint8_t* packet, size_t numberOfBytes );

void pqInit( uint32_t pqId, size_t elementSize, size_t numberOfElements, void (*cb)(uint8_t*,size_t) );


#endif

