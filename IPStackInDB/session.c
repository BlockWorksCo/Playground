



#include "session.h"
#include "udp.h"

#include <string.h>
#include <stdio.h>






void sessionProcessUDPPacket(IPv6Address* src, IPv6Address* dst, uint16_t srcPort, uint16_t dstPort, uint8_t* packet, size_t numberOfBytes )
{
    // Process payload
    uint8_t string[128] = {0};
    memcpy( &string[0], packet, numberOfBytes );
    printf("[%s]\n", string);

    //
    // echo the packet back.
    //
    uint8_t     response[128]   = {0};
    sprintf(response,"[%s]",string);
    encodeUDPFrame( src, dst, srcPort, dstPort, response, strlen(response) );
}
