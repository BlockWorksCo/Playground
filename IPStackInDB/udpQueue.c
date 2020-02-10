


#include "udp.h"
#include "udpQueue.h"
#include "ipv6.h"

#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <pthread.h>


#define MAX_STREAMS     (2)
#define QUEUE_DEPTH     (8)


typedef struct
{
    uint32_t    sequenceNumber;
    UDPHeader   udpHeader;

} ElementHeader;



int     fd[MAX_STREAMS]      = {0};


// Only address needs to be hashed because the packets still contain the 
// header which contains srcPort and dstPort.
// To ensure that there are no hash-collisions, we use 16-bits from the
// address and limit the address-range we can serve.
uint32_t ipAddressHash( IPv6Address* address )
{
    uint32_t    addressComponent    = (((uint32_t)((uint8_t*)address)[14])<<8) | (uint32_t)((uint8_t*)address)[15];
    if(addressComponent > MAX_UDPQUEUE_ADDRESS_RANGE) {
        // this address is out of our representible range.
        dumpHex((uint8_t*)address,16);
        printf("\naddress out of range (%04x).\n",addressComponent);
        exit(-1);
    }

    uint32_t    hash    = (addressComponent<<16) | 0x50 | 0x11;

    return hash;
}



void udpQueueGet( uint32_t streamId, uint8_t* packet, size_t* maxNumberOfBytes )
{
}


void udpQueuePut( uint32_t streamId, IPv6Address* src,  uint8_t* packet, size_t numberOfBytes )
{
    uint32_t    hash        = ipAddressHash(src);

    off_t       position    = hash * MAX_UDPQUEUE_ELEMENT_SIZE;
    lseek( fd[streamId], position, SEEK_SET );
    write( fd[streamId], packet, MAX_UDPQUEUE_ELEMENT_SIZE );
}


void *packetProcessorThread(void* param)
{
    while(true) {
        usleep(100);
    }
}


void udpQueueInit(uint32_t streamId)
{
    char    name[64]    = {0};

    sprintf(name,"/tmp/udpQueue%d.bin",streamId);
    fd[streamId]  = creat(name, O_RDWR);
    if( fd[streamId] == -1 ) {
        printf("\nCant open updQueue.\n");
        exit(-1);
    }

    // Initialise it.
    off_t    fileSize    = QUEUE_DEPTH * MAX_UDPQUEUE_ELEMENT_SIZE;
    lseek( fd[streamId], fileSize, SEEK_SET );
    uint8_t buf[MAX_UDPQUEUE_ELEMENT_SIZE]  = {0};
    write( fd[streamId], buf, sizeof(buf) );

    //
    pthread_t   threadId;
    if(pthread_create( &threadId, NULL, packetProcessorThread, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        exit(-1);
    }   
}


