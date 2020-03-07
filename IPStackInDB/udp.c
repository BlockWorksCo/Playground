

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
#include "session.h"
#include "PersistentQueue.h"


typedef struct
{
    // pseudo header
    IPv6Address src;
    IPv6Address dst;
    uint16_t    headerAndDataLength;
    uint16_t    nextHeader; // extended to 16-bit for calculation

    // UDP header
    uint16_t    srcPort;
    uint16_t    dstPort;
    uint16_t    length;
    uint16_t    checksum;

    // payload
    uint8_t     payload[1];

} __attribute__((__packed__)) UDPPsuedoHeader;



// Computing the internet checksum (RFC 1071).
// Note that the internet checksum does not preclude collisions.
uint16_t checksum (uint16_t* addr, int len)
{
    int count = len;
    register uint32_t sum = 0;
    uint16_t answer = 0;

    // Sum up 2-byte values until none or only one byte left.
    while (count > 1)
    {
        sum += *(addr++);
        count -= 2;
    }

    // Add left-over byte, if any.
    if (count > 0)
    {
        sum += *(uint8_t*) addr;
    }

    // Fold 32-bit sum into 16 bits; we lose information by doing this,
    // increasing the chances of a collision.
    // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
    while (sum >> 16)
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    // Checksum is one's compliment of sum.
    answer = ~sum;
    return (answer);
}

uint16_t udpChecksum( IPv6Address src, IPv6Address dst, uint16_t srcPort, uint16_t dstPort, uint16_t payloadLen, uint8_t* payload )
{
    char buf[2048]    = {0};
    UDPPsuedoHeader*  header  = (UDPPsuedoHeader*)&buf[0];
    int chksumlen = 0;
    int i;

    // Pseudo header
    printf("payloadLen=%d\n", payloadLen);
    // Copy source IP address into buf (128 bits)
    memcpy( &header->src, src, sizeof(IPv6Address) );
    chksumlen += sizeof (IPv6Address);
    // Copy destination IP address into buf (128 bits)
    memcpy( &header->dst, dst, sizeof(IPv6Address) );
    chksumlen += sizeof (IPv6Address);
    // Copy UDP length into buf (32 bits)
    header->headerAndDataLength   = htons(8 + payloadLen);
    chksumlen += sizeof (uint32_t);
    // Copy next header field to buf (extended to 16 bits)
    header->nextHeader    = htons(0x0011);
    chksumlen += sizeof (uint16_t);

    // UDP header.
    // Copy UDP source port to buf (16 bits)
    header->srcPort   = htons(srcPort);
    chksumlen += sizeof (uint16_t);
    // Copy UDP destination port to buf (16 bits)
    header->dstPort   = htons(dstPort);
    chksumlen += sizeof (uint16_t);
    // Copy UDP length again to buf (16 bits)
    header->length   = htons(8 + payloadLen);
    chksumlen += sizeof (uint16_t);
    // Copy UDP checksum to buf (16 bits)
    // Zero, since we don't know it yet
    header->checksum  = 0;
    chksumlen += sizeof (uint16_t);

    // payload.
    // Copy payload to buf
    memcpy( &header->payload[0], payload, payloadLen );
    chksumlen += payloadLen;

    // Pad to the next 16-bit boundary
    for (i = 0; i < payloadLen % 2; i++)
    {
        buf[chksumlen]  = 0x00;
        chksumlen++;
    }

    printf("src:");
    dumpHex( (uint8_t*)src, sizeof(IPv6Address) );
    printf("dst:");
    dumpHex( (uint8_t*)dst, sizeof(IPv6Address) );
    printf("checked frame:");
    dumpHex( (uint8_t*)buf, chksumlen );
    uint16_t checksumValue =  checksum((uint16_t*) buf, chksumlen);
    printf("checksumValue = %04x\n\n", htons(checksumValue));
    return htons(checksumValue);
}





void encodeUDPFrame( IPv6Address* src, IPv6Address* dst, uint16_t srcPort, uint16_t dstPort, uint8_t* payload, size_t payloadSize )
{
    uint8_t         packet[128] = {0};
    uint32_t   ipv6PacketLength    = 40 + 8 + payloadSize;
    printf("[ipv6PacketLength=%d]\n", ipv6PacketLength);
    packet[0]       = 0x60;
    packet[1]       = 0x0f;
    packet[2]       = 0x17;
    packet[3]       = 0xee;
    IPv6Address*    newSrc      = (IPv6Address*)&packet[8];
    IPv6Address*    newDst      = (IPv6Address*)&packet[24];
    uint16_t*       newIPv6PacketLength = (uint16_t*)&packet[4];
    uint8_t*        newNextHeader     = (uint8_t*)&packet[6];
    uint16_t*       newSrcPort  = (uint16_t*)&packet[40];
    uint16_t*       newDstPort  = (uint16_t*)&packet[42];
    uint16_t*       newUDPPacketLength = (uint16_t*)&packet[44];
    uint16_t*       newUDPCheckSum = (uint16_t*)&packet[46];
    uint8_t*        newUDPPayload  = &packet[48];
    memcpy( newSrc, dst, sizeof(IPv6Address) );
    memcpy( newDst, src, sizeof(IPv6Address) );
    *newIPv6PacketLength    = htons(payloadSize + 8);
    *newNextHeader  = 0x11;
    *newSrcPort     = htons(dstPort);
    *newDstPort     = htons(srcPort);
    *newUDPPacketLength = htons(payloadSize + 8);
    *newUDPCheckSum = 0x0000;
    memcpy( newUDPPayload, payload, payloadSize );
    *newUDPCheckSum = htons(udpChecksum( *newSrc, *newDst, ntohs(*newSrcPort), ntohs(*newDstPort), payloadSize, newUDPPayload ));
    printf("outgoing frame:");
    dumpHex( packet, ipv6PacketLength );
    // transmit the packet.
    pqPut( 1, &packet[0], ipv6PacketLength );
    //uint16_t nwrite = cwrite(tun_fd, &packet[0], ipv6PacketLength);
    printf("replied...\n");
}


void decodeUDPFrame( IPv6Address* src, IPv6Address* dst, uint8_t* frame, size_t numberOfBytes )
{
    // UDP
    uint16_t        srcPort         = ntohs(*(uint16_t*)&frame[40]);
    uint16_t        dstPort         = ntohs(*(uint16_t*)&frame[42]);
    uint16_t        udpPacketLength = ntohs(*(uint16_t*)&frame[44]);
    uint16_t        udpCheckSum     = ntohs(*(uint16_t*)&frame[46]);    // assume its zero/unused.
    uint8_t*        udpPayload  = &frame[48];
    printf("UDP: srcPort=%d dstPort=%d udpLength=%d udpChecksum=%04x\n", srcPort, dstPort, udpPacketLength, udpCheckSum );

    // UDP packets sent to port 80.
    if(dstPort == 80)
    {
        printf("incoming frame:");
        dumpHex( frame, numberOfBytes );

        // Integrity check.
        uint16_t checkValue = udpChecksum( *src, *dst, srcPort, dstPort, udpPacketLength - 8, udpPayload );
        printf("\n[%04x == %04x]\n", udpCheckSum, checkValue);
    }

    // DTLS packets sent to port 11111.
    if(dstPort == 11111) {

        printf("<DTLS packet received>\n");

        // Pass it up the layer stack.
        sessionProcessUDPPacket( src, dst, srcPort, dstPort, udpPayload, udpPacketLength-8 );
    }
}



void udpInit()
{
    sessionInit();
}

