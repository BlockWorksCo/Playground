

#ifndef __SESSION_H__
#define __SESSION_H__



#include "ipv6.h"
#include <stdint.h>


void sessionInit();

void sessionProcessUDPPacket(IPv6Address* src, IPv6Address* dst, uint16_t srcPort, uint16_t dstPort, uint8_t* packet, size_t numberOfBytes );

#endif

