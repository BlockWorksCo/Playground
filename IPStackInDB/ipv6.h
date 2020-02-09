

#ifndef __IPV6_H__
#define __IPV6_H__

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t IPv6Address[16];

void dumpHex( uint8_t* bytes, size_t numberOfBytes );
void encodeUDPFrame( IPv6Address* src, IPv6Address* dst, uint16_t srcPort, uint16_t dstPort, uint8_t* payload, size_t payloadSize );
void decodeFrame( uint8_t* frame, size_t numberOfBytes );

#endif

