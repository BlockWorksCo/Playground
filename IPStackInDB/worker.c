


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

#define MAX_WORKERS     (2)


typedef struct
{
    uint32_t    sequenceNumber;
    uint32_t    checksum;

} SlotHeader;


typedef struct
{
    void        (*doWork)(uint8_t* packet);
    int         fd;
    uint32_t    firstEmptySlot;
    uint32_t    firstFullSlot;
    sem_t       dataAvailableSemaphore;

} WorkerContext;

WorkerContext   context[MAX_WORKERS]            = {0};



uint32_t checksumOf( uint8_t* element )
{
    uint32_t    sum = 0;
    for(uint32_t i=0; i<MAX_UDPQUEUE_ELEMENT_SIZE-sizeof(SlotHeader); i++) {
        sum += (uint32_t)element[i];
    }

    return sum;
}


void workerPut( uint32_t workerId, IPv6Address* src,  uint8_t* packet, size_t numberOfBytes )
{
    lseek( context[workerId].fd, MAX_UDPQUEUE_ELEMENT_SIZE*context[workerId].firstEmptySlot, SEEK_SET );

    SlotHeader   header  = 
    {
        .sequenceNumber = 0,
        .checksum       = checksumOf(packet),
    };
    write( context[workerId].fd, &header, sizeof(SlotHeader) );
    write( context[workerId].fd, packet, MAX_UDPQUEUE_ELEMENT_SIZE-sizeof(SlotHeader) );

    context[workerId].firstEmptySlot  = (context[workerId].firstEmptySlot+1) % QUEUE_DEPTH;
    sem_post( &dataAvailableSemaphore[workerId] );
}


void *workerThread(void* param)
{
    Context*    context = (WorkerContext*)param;

    while(true) {
        sem_wait( &context->dataAvailableSemaphore );
        printf("\n****[data available]****\n");

        // Read all available packets.
        while( context->firstFullSlot != context->firstEmptySlot ) {

            uint8_t element[MAX_UDPQUEUE_ELEMENT_SIZE];
            SlotHeader*  header  = (SlotHeader*)element;

            lseek( context->fd, MAX_UDPQUEUE_ELEMENT_SIZE*context->firstFullSlot, SEEK_SET );
            read( context->fd, &element[0], MAX_UDPQUEUE_ELEMENT_SIZE );

            // process it...
            if(checksumOf(&element[sizeof(SlotHeader)]) == header->checksum) {

                // Good checksum, so process it.
                context->doWork( &element[sizeof(SlotHeader)] );
            }
            else {
                printf("\n!! bad checksum on packet !!\n");
            }

            // mark it as bad.
            memset( element, 0xff, 16 );
            lseek( context[workerId].fd, MAX_UDPQUEUE_ELEMENT_SIZE*context[workerId].firstFullSlot, SEEK_SET );
            write( context[workerId].fd, element, MAX_UDPQUEUE_ELEMENT_SIZE );

            context[workerId].firstFullSlot  = (context[workerId].firstFullSlot+1) % QUEUE_DEPTH;
        }
    }
}



void workerInit(uint32_t workerId, void (*doWork)(uint8_t*) )
{
    char    name[64]    = {0};

    sprintf(name,"/tmp/udpQueue%d.bin",workerId);
    context[workerId].fd  = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG);
    if( context[workerId].fd == -1 ) {
        printf("\nCant open worker.\n");
        exit(-1);
    }

    // Initialise it.
    off_t    fileSize    = QUEUE_DEPTH * MAX_UDPQUEUE_ELEMENT_SIZE;
    lseek( context[workerId].fd, fileSize, SEEK_SET );
    uint8_t buf[MAX_UDPQUEUE_ELEMENT_SIZE]  = {0};
    write( context[workerId].fd, buf, sizeof(buf) );

    // Find the end of the queue (the highest-numbered element with
    // a good checksum)...
    int32_t     maxSequenceNumber   = -1;
    uint32_t    maxSequencePosition = -1;
    int32_t     minSequenceNumber   = -1;
    uint32_t    minSequencePosition = -1;
    for(uint32_t i=0; i<QUEUE_DEPTH; i++) {

        uint8_t element[MAX_UDPQUEUE_ELEMENT_SIZE];
        SlotHeader*  header  = (SlotHeader*)element;
        lseek( context[workerId].fd, MAX_UDPQUEUE_ELEMENT_SIZE*i, SEEK_SET );
        read( context[workerId].fd, &buf[0], MAX_UDPQUEUE_ELEMENT_SIZE );

        if((header->sequenceNumber >= maxSequenceNumber) && 
           (checksumOf(&buf[sizeof(SlotHeader)]) == header->checksum)) {
            maxSequenceNumber   = header->sequenceNumber;
            maxSequencePosition = i;
        }

        if((header->sequenceNumber <= minSequenceNumber) && 
           (checksumOf(&buf[sizeof(SlotHeader)]) == header->checksum)) {
            minSequenceNumber   = header->sequenceNumber;
            minSequencePosition = i;
        }
    }

    if(maxSequenceNumber == -1) {
        context[workerId].firstEmptySlot = 0;
        context[workerId].firstFullSlot  = 0;
    }
    else {
        // ...then store the first empty slot index.
        context[workerId].firstEmptySlot = (maxSequencePosition+1)%QUEUE_DEPTH;

        // ...and the first full slot index.
        context[workerId].firstFullSlot = minSequencePosition;
    }

    //
    sem_init( &context[workerId].dataAvailableSemaphore, 0, 0 );

    // Start a reader thread.
    pthread_t   threadId;
    if(pthread_create( &threadId, NULL, workerThread, (void*)&context[workerId])) {
        fprintf(stderr, "Error creating thread\n");
        exit(-1);
    }   
}


