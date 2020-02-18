


#include "udp.h"
#include "udpQueue.h"
#include "ipv6.h"

#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>


#define MAX_PQ      (128)


struct 
{
    int         fd;
    size_t      elementSize;
    size_t      numberOfElements;
    uint32_t    firstEmptyElement;
    uint32_t    firstFullElement;
    sem_t       dataAvailableSemaphore;

} pqContext[MAX_PQ];


typedef struct
{
    uint32_t    sequenceNumber;
    uint32_t    checksum;

} ElementHeader;




uint32_t checksumOf( uint8_t* element );



void udpQueuePut( uint32_t pqId, IPv6Address* src,  uint8_t* packet, size_t numberOfBytes )
{
    lseek( pqCcontext[pqId].fd, MAX_UDPQUEUE_ELEMENT_SIZE*firstEmptyElement[pqId], SEEK_SET );

    ElementHeader   header  = 
    {
        .sequenceNumber = 0,
        .checksum       = checksumOf(packet),
    };
    write( pqCcontext[pqId].fd, &header, sizeof(ElementHeader) );
    write( pqCcontext[pqId].fd, packet, MAX_UDPQUEUE_ELEMENT_SIZE-sizeof(ElementHeader) );

    firstEmptyElement[pqId]  = (firstEmptyElement[pqId]+1) % QUEUE_DEPTH;
    sem_post( &dataAvailableSemaphore[pqId] );
}


void processElement( uint32_t pqId, uint8_t* element )
{
    printf("processing packet...\n");
}


void *packetProcessorThread(void* param)
{
    uint32_t    pqId    = (uint32_t)param;

    while(true) {
        sem_wait( &dataAvailableSemaphore[pqId] );
        printf("\n****[data available]****\n");

        // Read all available packets.
        while( firstFullElement[pqId] != firstEmptyElement[pqId] ) {

            uint8_t element[MAX_UDPQUEUE_ELEMENT_SIZE];
            ElementHeader*  header  = (ElementHeader*)element;

            lseek( pqCcontext[pqId].fd, MAX_UDPQUEUE_ELEMENT_SIZE*firstFullElement[pqId], SEEK_SET );
            read( pqCcontext[pqId].fd, &element[0], MAX_UDPQUEUE_ELEMENT_SIZE );

            // process it...
            if(checksumOf(&element[sizeof(ElementHeader)]) == header->checksum) {
                processElement( pqId, &element[sizeof(ElementHeader)]);
            }
            else {
                printf("\n!! bad checksum on packet !!\n");
            }

            // mark it as bad.
            memset( element, 0xff, 16 );
            lseek( pqCcontext[pqId].fd, MAX_UDPQUEUE_ELEMENT_SIZE*firstFullElement[pqId], SEEK_SET );
            write( pqCcontext[pqId].fd, element, MAX_UDPQUEUE_ELEMENT_SIZE );

            firstFullElement[pqId]  = (firstFullElement[pqId]+1) % QUEUE_DEPTH;
        }
    }
}



uint32_t checksumOf( uint8_t* element )
{
    uint32_t    sum = 0;
    for(uint32_t i=0; i<MAX_UDPQUEUE_ELEMENT_SIZE-sizeof(ElementHeader); i++) {
        sum += (uint32_t)element[i];
    }

    return sum;
}



void pqQueueInit( uint32_t pqId, size_t elementSize, size_t numberOfElements )
{
    // Determine the name of the backing store for this q and open
    // the file for it.
    char    name[64]    = {0};

    sprintf(name,"/tmp/pq%d.bin",pqId);
    pqContext[pqId].fd  = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG);
    if( pqCcontext[pqId].fd == -1 ) {
        printf("\nCant open pq.\n");
        exit(-1);
    }

    // Initialise the context.
    pqContext[pqId].elementSize         = elementSize;
    pqContext[pqId].numberOfElements    = numberOfelements;

    // Initialise the backing store by writing a byte 1 byte past
    // the calculated end.
    off_t    fileSize    = QUEUE_DEPTH * MAX_UDPQUEUE_ELEMENT_SIZE;
    lseek( pqContext[pqId].fd, fileSize+1, SEEK_SET );
    uint8_t temp    = 0;
    write( pqContext[pqId].fd, &temp, 1 );

    // Find the end of the queue (the highest-numbered element with
    // a good checksum)...
    int32_t     maxSequenceNumber   = -1;
    uint32_t    maxSequencePosition = -1;
    int32_t     minSequenceNumber   = -1;
    uint32_t    minSequencePosition = -1;
    uint8_t     buf[MAX_UDPQUEUE_ELEMENT_SIZE]  = {0};
    for(uint32_t i=0; i<QUEUE_DEPTH; i++) {

        uint8_t element[MAX_UDPQUEUE_ELEMENT_SIZE];
        ElementHeader*  header  = (ElementHeader*)element;
        lseek( pqCcontext[pqId].fd, MAX_UDPQUEUE_ELEMENT_SIZE*i, SEEK_SET );
        read( pqCcontext[pqId].fd, &buf[0], MAX_UDPQUEUE_ELEMENT_SIZE );

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

    if(maxSequenceNumber == -1) {
        firstEmptyElement[pqId] = 0;
        firstFullElement[pqId]  = 0;
    }
    else {
        // ...then store the first empty slot index.
        firstEmptyElement[pqId] = (maxSequencePosition+1)%QUEUE_DEPTH;

        // ...and the first full slot index.
        firstFullElement[pqId] = minSequencePosition;
    }

    //
    sem_init( &dataAvailableSemaphore[pqId], 0, 0 );

    // Start a reader thread.
    pthread_t   threadId;
    if(pthread_create( &threadId, NULL, packetProcessorThread, (void*)pqId)) {
        fprintf(stderr, "Error creating thread\n");
        exit(-1);
    }   
}


