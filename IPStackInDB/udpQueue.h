


#ifndef __UDPQUEUE_H__
#define __UDPQUEUE_H__


#include <stdint.h>
#include <stdlib.h>

#include "ipv6.h"


#define MAX_UDPQUEUE_ADDRESS_RANGE      (1024)
#define MAX_UDPQUEUE_ELEMENT_SIZE       (1024*2)



void udpQueueGet( uint8_t* packet, size_t* maxNumberOfBytes );

void udpQueuePut( IPv6Address* src, uint8_t* packet, size_t numberOfBytes );

void udpQueueInit();


#endif

