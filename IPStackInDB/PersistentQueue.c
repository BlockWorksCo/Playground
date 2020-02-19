


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


struct PQContext
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
    lseek( pqContext[pqId].fd, MAX_UDPQUEUE_ELEMENT_SIZE*pqContext[pqId].firstEmptyElement, SEEK_SET );

    ElementHeader   header  = 
    {
        .sequenceNumber = 0,
        .checksum       = checksumOf(packet),
    };
    write( pqContext[pqId].fd, &header, sizeof(ElementHeader) );
    write( pqContext[pqId].fd, packet, MAX_UDPQUEUE_ELEMENT_SIZE-sizeof(ElementHeader) );

    pqContext[pqId].firstEmptyElement  = (pqContext[pqId].firstEmptyElement+1) % pqContext[pqId].numberOfElements;
    sem_post( &pqContext[pqId].dataAvailableSemaphore );
}


void processElement( uint8_t* element )
{
    printf("processing packet...\n");
}


void *packetProcessorThread(void* param)
{
    struct PQContext*   context = (struct PQContext*)param;

    while(true) {
        sem_wait( &context->dataAvailableSemaphore );
        printf("\n****[data available]****\n");

        // Read all available packets.
        while( context->firstFullElement != context->firstEmptyElement ) {

            uint8_t element[context->elementSize];
            ElementHeader*  header  = (ElementHeader*)element;

            lseek( context->fd, context->elementSize*context->firstFullElement, SEEK_SET );
            read( context->fd, &element[0], context->elementSize );

            // process it...
            if(checksumOf(&element[sizeof(ElementHeader)]) == header->checksum) {
                processElement( &element[sizeof(ElementHeader)] );
            }
            else {
                printf("\n!! bad checksum on packet !!\n");
            }

            // mark it as bad.
            memset( element, 0xff, 16 );
            lseek( context->fd, context->elementSize*context->firstFullElement, SEEK_SET );
            write( context->fd, element, context->elementSize );

            context->firstFullElement  = (context->firstFullElement+1) % context->numberOfElements;
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
    if( pqContext[pqId].fd == -1 ) {
        printf("\nCant open pq.\n");
        exit(-1);
    }

    // Initialise the context.
    pqContext[pqId].elementSize         = elementSize;
    pqContext[pqId].numberOfElements    = numberOfElements;

    // Initialise the backing store by writing a byte 1 byte past
    // the calculated end.
    off_t    fileSize    = pqContext[pqId].elementSize * pqContext[pqId].numberOfElements;
    lseek( pqContext[pqId].fd, fileSize+1, SEEK_SET );
    uint8_t temp    = 0;
    write( pqContext[pqId].fd, &temp, 1 );

    // Find the end of the queue (the highest-numbered element with
    // a good checksum)...
    int32_t     maxSequenceNumber   = -1;
    uint32_t    maxSequencePosition = -1;
    int32_t     minSequenceNumber   = -1;
    uint32_t    minSequencePosition = -1;
    uint8_t     buf[pqContext[pqId].elementSize];

    // For each element...
    for(uint32_t i=0; i<pqContext[pqId].numberOfElements; i++) {

        // read in the element.
        ElementHeader*  header  = (ElementHeader*)&buf[0];
        lseek( pqContext[pqId].fd, pqContext[pqId].elementSize*i, SEEK_SET );
        read( pqContext[pqId].fd, &buf[0], pqContext[pqId].elementSize );

        // check if its good and if so, then is its sequence number increasing?
        if((header->sequenceNumber >= maxSequenceNumber) && 
           (checksumOf(&buf[sizeof(ElementHeader)]) == header->checksum)) {
            maxSequenceNumber   = header->sequenceNumber;
            maxSequencePosition = i;
        }

        // check if its good and if so, then is its sequence number decreasing?
        if((header->sequenceNumber <= minSequenceNumber) && 
           (checksumOf(&buf[sizeof(ElementHeader)]) == header->checksum)) {
            minSequenceNumber   = header->sequenceNumber;
            minSequencePosition = i;
        }
    }

    // Check if we found a good element or not...
    if(maxSequenceNumber == -1) {
        // No good elements, so set everything to init position.
        pqContext[pqId].firstEmptyElement = 0;
        pqContext[pqId].firstFullElement  = 0;
    }
    else {
        // We have good elements already, so init the pointers.
        pqContext[pqId].firstEmptyElement = (maxSequencePosition+1)%pqContext[pqId].numberOfElements;;
        pqContext[pqId].firstFullElement = minSequencePosition;
    }

    // Create a semaphore for triggering the reader.
    sem_init( &pqContext[pqId].dataAvailableSemaphore, 0, 0 );

    // Start a reader thread.
    pthread_t   threadId;
    if(pthread_create( &threadId, NULL, packetProcessorThread, (void*)&pqContext[pqId])) {
        fprintf(stderr, "Error creating thread\n");
        exit(-1);
    }   
}


