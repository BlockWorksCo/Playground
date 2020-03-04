

#ifndef __IPV6_H__
#define __IPV6_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint8_t IPv6Address[16];

void ipv6Init();
void dumpHex( uint8_t* bytes, size_t numberOfBytes );
void decodeFrame( uint8_t* frame, size_t numberOfBytes );

#endif

