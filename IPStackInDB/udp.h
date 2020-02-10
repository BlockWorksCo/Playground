
#ifndef __UDP_H__
#define __UDP_H__

#include "ipv6.h"

typedef struct
{
    uint16_t    srcPort;
    uint16_t    dstPort;
    uint16_t    numberOfBytes;
    uint16_t    checksum;

} UDPHeader;

void encodeUDPFrame( IPv6Address* src, IPv6Address* dst, uint16_t srcPort, uint16_t dstPort, uint8_t* payload, size_t payloadSize );
void decodeUDPFrame( IPv6Address* src, IPv6Address* dst, uint8_t* frame, size_t numberOfBytes );


#endif
