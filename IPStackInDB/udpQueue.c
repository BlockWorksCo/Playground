


#include "udpQueue.h"
#include "ipv6.h"

#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>



int     fd      = -1;


// Only address needs to be hashed because the packets still contain the 
// header which contains srcPort and dstPort.
// To ensure that there are no hash-collisions, we use 16-bits from the
// address and limit the address-range we can serve.
uint32_t ipAddressHash( IPv6Address* address )
{
    uint32_t    addressComponent    = (((uint32_t)address[14])<<8) | (uint32_t)address[15];
    if(addressComponent > MAX_UDPQUEUE_ADDRESS_RANGE) {
        // this address is out of our representible range.
        printf("\naddress out of range.\n");
        exit(-1);
    }

    uint32_t    hash    = (addressComponent<<16) | 0x50 | 0x11;

    return hash;
}



void udpQueueGet( uint8_t* packet, size_t* maxNumberOfBytes )
{
}


void udpQueuePut( IPv6Address* src,  uint8_t* packet, size_t numberOfBytes )
{
    uint32_t    hash        = ipAddressHash(src);

    off_t       position    = hash * MAX_UDPQUEUE_ELEMENT_SIZE;
    lseek( fd, position, SEEK_SET );
    write( fd, packet, MAX_UDPQUEUE_ELEMENT_SIZE );
}



void udpQueueInit()
{
    fd  = creat("/tmp/udpQueue.bin", O_RDWR);
    if( fd == -1 ) {
        printf("\nCant open updQueue.\n");
        exit(-1);
    }

    // Initialise it.
    off_t    fileSize    = MAX_UDPQUEUE_ADDRESS_RANGE * MAX_UDPQUEUE_ELEMENT_SIZE;
    lseek( fd, fileSize, SEEK_SET );
    uint8_t buf[MAX_UDPQUEUE_ELEMENT_SIZE]  = {0};
    write( fd, buf, sizeof(buf) );
}


