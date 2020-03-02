

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>


#include "layer1.h"
#include "ipv6.h"
#include "tun.h"
#include "udp.h"



void dumpHex( uint8_t* bytes, size_t numberOfBytes )
{
    for(uint32_t i = 0; i < numberOfBytes; i++)
    {
        printf("%02x ", bytes[i]);
    }

    printf("\n");
}


void decodeFrame( uint8_t* frame, size_t numberOfBytes )
{
    printf("Decoding frame [%zd]\n", numberOfBytes);
    uint8_t frameType   = frame[0] & 0xe0;

    if(frameType == 0x60)
    {
        printf("IPv6\n");
    }

    if(frameType == 0x40)
    {
        printf("IPv4\n");
    }

    uint16_t        ipv6PacketLength = ntohs(*(uint16_t*)&frame[4]);
    uint8_t         nextHeader          = *(uint8_t*)&frame[6];
    IPv6Address*    src = (IPv6Address*)&frame[8];
    IPv6Address*    dst = (IPv6Address*)&frame[24];

    if( nextHeader == 0x3a )
    {
        // ICMP
        printf("[ICMP packet]\n");
    }

    if( nextHeader == 0x11 )
    {
        decodeUDPFrame( src, dst, frame,numberOfBytes);
    }
}



void encodeFrame( uint8_t* frame, size_t numberOfBytes )
{
}



