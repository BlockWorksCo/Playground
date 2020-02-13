


#include "udp.h"
#include "udpQueue.h"
#include "ipv6.h"

#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>


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



void udpQueuePut( uint32_t streamId, IPv6Address* src,  uint8_t* packet, size_t numberOfBytes )
{
    lseek( fd[streamId], MAX_UDPQUEUE_ELEMENT_SIZE*firstEmptyElement[streamId], SEEK_SET );

    ElementHeader   header  = 
    {
        .sequenceNumber = 0,
        .checksum       = checksumOf(packet),
    };
    write( fd[streamId], &header, sizeof(ElementHeader) );
    write( fd[streamId], packet, MAX_UDPQUEUE_ELEMENT_SIZE-sizeof(ElementHeader) );

    firstEmptyElement[streamId]  = (firstEmptyElement[streamId]+1) % QUEUE_DEPTH;
    sem_post( &dataAvailableSemaphore[streamId] );
}


void processElement( uint32_t streamId, uint8_t* element )
{
    printf("processing packet...\n");
}


void *packetProcessorThread(void* param)
{
    uint32_t    streamId    = (uint32_t)param;

    while(true) {
        sem_wait( &dataAvailableSemaphore[streamId] );
        printf("\n****[data available]****\n");

        // Read all available packets.
        while( firstFullElement[streamId] != firstEmptyElement[streamId] ) {

            uint8_t element[MAX_UDPQUEUE_ELEMENT_SIZE];
            ElementHeader*  header  = (ElementHeader*)element;

            lseek( fd[streamId], MAX_UDPQUEUE_ELEMENT_SIZE*firstFullElement[streamId], SEEK_SET );
            read( fd[streamId], &element[0], MAX_UDPQUEUE_ELEMENT_SIZE );

            // process it...
            if(checksumOf(&element[sizeof(ElementHeader)]) == header->checksum) {
                processElement( streamId, &element[sizeof(ElementHeader)]);
            }
            else {
                printf("\n!! bad checksum on packet !!\n");
            }

            // mark it as bad.
            memset( element, 0xff, 16 );
            lseek( fd[streamId], MAX_UDPQUEUE_ELEMENT_SIZE*firstFullElement[streamId], SEEK_SET );
            write( fd[streamId], element, MAX_UDPQUEUE_ELEMENT_SIZE );

            firstFullElement[streamId]  = (firstFullElement[streamId]+1) % QUEUE_DEPTH;
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



void udpQueueInit(uint32_t streamId)
{
    char    name[64]    = {0};

    sprintf(name,"/tmp/udpQueue%d.bin",streamId);
    fd[streamId]  = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG);
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
        lseek( fd[streamId], MAX_UDPQUEUE_ELEMENT_SIZE*i, SEEK_SET );
        read( fd[streamId], &buf[0], MAX_UDPQUEUE_ELEMENT_SIZE );

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
        firstEmptyElement[streamId] = 0;
        firstFullElement[streamId]  = 0;
    }
    else {
        // ...then store the first empty slot index.
        firstEmptyElement[streamId] = (maxSequencePosition+1)%QUEUE_DEPTH;

        // ...and the first full slot index.
        firstFullElement[streamId] = minSequencePosition;
    }

    //
    sem_init( &dataAvailableSemaphore[streamId], 0, 0 );

    // Start a reader thread.
    pthread_t   threadId;
    if(pthread_create( &threadId, NULL, packetProcessorThread, (void*)streamId)) {
        fprintf(stderr, "Error creating thread\n");
        exit(-1);
    }   
}


