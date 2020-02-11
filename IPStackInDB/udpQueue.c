


#include "udp.h"
#include "udpQueue.h"
#include "ipv6.h"

#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


#define MAX_STREAMS     (2)
#define QUEUE_DEPTH     (8)


typedef struct
{
    uint32_t    sequenceNumber;
    uint32_t    checksum;

} ElementHeader;



int         fd[MAX_STREAMS]                     = {0};
uint32_t    firstEmptyElement[MAX_STREAMS]      = {0};
uint32_t    firstFullElement[MAX_STREAMS]       = {0};
sem_t       dataAvailableSemaphore[MAX_STREAMS] = {0};

uint32_t checksumOf( uint8_t* element );
void readElement( uint32_t streamId, uint32_t elementId, uint8_t* element );



// Only address needs to be hashed because the packets still contain the 
// header which contains srcPort and dstPort.
// To ensure that there are no hash-collisions, we use 16-bits from the
// address and limit the address-range we can serve.
uint32_t ipAddressHash( IPv6Address* address )
{
#if 0
    uint32_t    addressComponent    = (((uint32_t)((uint8_t*)address)[14])<<8) | (uint32_t)((uint8_t*)address)[15];
    if(addressComponent > MAX_UDPQUEUE_ADDRESS_RANGE) {
        // this address is out of our representible range.
        dumpHex((uint8_t*)address,16);
        printf("\naddress out of range (%04x).\n",addressComponent);
        exit(-1);
    }

    uint32_t    hash    = (addressComponent<<16) | 0x50 | 0x11;
#endif
    uint32_t    addressComponent    = ((uint8_t*)address)[15];
    uint32_t    hash    = addressComponent & 0x01;

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

    ElementHeader   header  = 
    {
        .sequenceNumber = 0,
        .checksum       = checksumOf(packet),
    };
    write( fd[streamId], &header, sizeof(ElementHeader) );
    write( fd[streamId], packet, MAX_UDPQUEUE_ELEMENT_SIZE );

    sem_post( &dataAvailableSemaphore[streamId] );
}


void *packetProcessorThread(void* param)
{
    uint32_t    streamId    = (uint32_t)param;

    while(true) {
        sem_wait( &dataAvailableSemaphore[streamId] );
        printf("\n****[data available]****\n");

        // Read all available packets.
        while( firstFullElement[streamId] |= firstEmptyElement[streamId] ) {
            uint8_t element[MAX_UDPQUEUE_ELEMENT_SIZE];
            ElementHeader*  header  = (ElementHeader*)element;
            readElement( streamId, firstFullElement[streamId], &element[0] );

            firstFullElement[streamId]  = (firstFullElement[streamId]+1) % QUEUE_DEPTH;
        }
    }
}


void readElement( uint32_t streamId, uint32_t elementId, uint8_t* element )
{
    lseek( fd[streamId], MAX_UDPQUEUE_ELEMENT_SIZE*elementId, SEEK_SET );
    read( fd[streamId], element, MAX_UDPQUEUE_ELEMENT_SIZE );
}


uint32_t checksumOf( uint8_t* element )
{
    uint32_t    sum = 0;
    for(uint32_t i=0; i<MAX_UDPQUEUE_ELEMENT_SIZE-sizeof(ElementHeader); i++) {
        sum += (uint32_t)element[i];
    }

    return sum;
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

    // Find the end of the queue (the highest-numbered element with
    // a good checksum)...
    int32_t     maxSequenceNumber   = -1;
    uint32_t    maxSequencePosition = -1;
    int32_t     minSequenceNumber   = -1;
    uint32_t    minSequencePosition = -1;
    for(uint32_t i=0; i<QUEUE_DEPTH; i++) {
        uint8_t element[MAX_UDPQUEUE_ELEMENT_SIZE];
        ElementHeader*  header  = (ElementHeader*)element;
        readElement( streamId, i, &buf[0] );

        if((header->sequenceNumber >= maxSequenceNumber) && 
           (checksumOf(&buf[sizeof(ElementHeader)]) == header->checksum)) {
            maxSequenceNumber   = header->sequenceNumber;
            maxSequencePosition = i;
        }

        if((header->sequenceNumber <= minSequenceNumber) && 
           (checksumOf(&buf[sizeof(ElementHeader)]) == header->checksum)) {
            minSequenceNumber   = header->sequenceNumber;
            minSequencePosition = i;
        }
    }

    // ...then store the first empty slot index.
    firstEmptyElement[streamId] = (maxSequencePosition+1)%QUEUE_DEPTH;

    // ...and the first full slot index.
    firstFullElement[streamId] = minSequencePosition;

    //
    sem_init( &dataAvailableSemaphore[streamId], 0, 1 );

    // Start a reader thread.
    pthread_t   threadId;
    if(pthread_create( &threadId, NULL, packetProcessorThread, (void*)streamId)) {
        fprintf(stderr, "Error creating thread\n");
        exit(-1);
    }   
}


