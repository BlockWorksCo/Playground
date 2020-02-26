


#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>


#define MAX_PQ      (128)


struct PQContext
{
    int         fd;
    size_t      elementSize;
    size_t      payloadSize;
    size_t      numberOfElements;
    uint32_t    firstEmptyElement;
    uint32_t    firstFullElement;
    sem_t       dataAvailableSemaphore;
    void        (*process)(uint8_t*, size_t);
    uint32_t    nextSequenceElementToWrite;

    pthread_mutex_t lock;

} pqContext[MAX_PQ];


typedef struct
{
    uint32_t    sequenceNumber;
    uint32_t    checksum;

} __attribute__((packed)) ElementHeader;




static uint32_t checksumOf( uint8_t* element, size_t numberOfBytes );



void pqPut( uint32_t pqId, uint8_t* packet, size_t numberOfBytes )
{
    //printf("ipqContext[pqId].nextSequenceElementToWrite=%d\n",pqContext[pqId].nextSequenceElementToWrite);
    ElementHeader   header  = 
    {
        .sequenceNumber = pqContext[pqId].nextSequenceElementToWrite,
        .checksum       = checksumOf(packet,pqContext[pqId].payloadSize),
    };

    pthread_mutex_lock(&pqContext[pqId].lock);

    lseek( pqContext[pqId].fd, pqContext[pqId].elementSize*pqContext[pqId].firstEmptyElement, SEEK_SET );
    write( pqContext[pqId].fd, &header, sizeof(ElementHeader) );
    write( pqContext[pqId].fd, packet, pqContext[pqId].payloadSize );
    //printf("sizeof(ElementHeader)=%ld\n",sizeof(ElementHeader));
    //printf("pqContext[pqId].payloadSize=%ld\n",pqContext[pqId].payloadSize);

    pqContext[pqId].firstEmptyElement           = (pqContext[pqId].firstEmptyElement+1) % pqContext[pqId].numberOfElements;
    pqContext[pqId].nextSequenceElementToWrite  = pqContext[pqId].nextSequenceElementToWrite+1;

    pthread_mutex_unlock(&pqContext[pqId].lock);

    sem_post( &pqContext[pqId].dataAvailableSemaphore );
}


static void *packetProcessorThread(void* param)
{
    struct PQContext*   context = (struct PQContext*)param;

    while(true) {
        sem_wait( &context->dataAvailableSemaphore );

        // Read all available packets.
        while( context->firstFullElement != context->firstEmptyElement ) {

            uint8_t element[context->elementSize];
            ElementHeader*  header  = (ElementHeader*)element;
            uint8_t*        payload = &element[sizeof(ElementHeader)];

            pthread_mutex_lock(&context->lock);

            lseek( context->fd, context->elementSize*context->firstFullElement, SEEK_SET );
            read( context->fd, &element[0], context->elementSize );

            // process it...
            if(checksumOf(payload, context->payloadSize) == header->checksum) {
                //printf("\n****[data available %"PRIu32"]****\n",header->sequenceNumber);
                context->process( payload, context->payloadSize );
            }
            else {
                printf("\n!! bad checksum on packet in slot %"PRIu32" !!\n", context->firstFullElement);
            }

            // mark it as bad.
            memset( element, 0xff, sizeof(ElementHeader) );
            lseek( context->fd, context->elementSize*context->firstFullElement, SEEK_SET );
            //write( context->fd, element, context->elementSize );

            context->firstFullElement  = (context->firstFullElement+1) % context->numberOfElements;
            pthread_mutex_unlock(&context->lock);
        }
    }
}



static uint32_t checksumOf( uint8_t* element, size_t numberOfBytes )
{
    uint32_t    sum = 0;
    for(uint32_t i=0; i<numberOfBytes; i++) {
        sum += (uint32_t)element[i];
    }

    return sum;
}



void pqInit( uint32_t pqId, size_t elementSize, size_t numberOfElements, void (*cb)(uint8_t*,size_t) )
{
    // Adjust for header size.
    elementSize += sizeof(ElementHeader);

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
    pqContext[pqId].payloadSize         = elementSize-sizeof(ElementHeader);
    pqContext[pqId].numberOfElements    = numberOfElements;
    pqContext[pqId].process             = cb;

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
    int32_t     minSequenceNumber   = INT32_MAX;
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
           (checksumOf(&buf[sizeof(ElementHeader)],pqContext[pqId].payloadSize) == header->checksum)) {
            maxSequenceNumber   = header->sequenceNumber;
            maxSequencePosition = i;
        }

        // check if its good and if so, then is its sequence number decreasing?
        if((header->sequenceNumber <= minSequenceNumber) && 
           (checksumOf(&buf[sizeof(ElementHeader)],pqContext[pqId].payloadSize) == header->checksum)) {
            minSequenceNumber   = header->sequenceNumber;
            minSequencePosition = i;
        }
    }

    // Check if we found a good element or not...
    if(maxSequenceNumber == -1) {
        // No good elements, so set everything to init position.
        pqContext[pqId].firstEmptyElement = 0;
        pqContext[pqId].firstFullElement  = 0;
        pqContext[pqId].nextSequenceElementToWrite  = 0;
    }
    else {
        // We have good elements already, so init the pointers.
        pqContext[pqId].firstEmptyElement = (maxSequencePosition+1)%pqContext[pqId].numberOfElements;;
        pqContext[pqId].firstFullElement = minSequencePosition;
        pqContext[pqId].nextSequenceElementToWrite  = maxSequenceNumber+1;
    }

    printf("1st empty=%"PRIu32"\n",pqContext[pqId].firstEmptyElement);
    printf("1st full=%"PRIu32"\n",pqContext[pqId].firstFullElement);
    printf("nextSeqNum=%"PRIu32"\n",pqContext[pqId].nextSequenceElementToWrite);

    // Create a semaphore for triggering the reader.
    sem_init( &pqContext[pqId].dataAvailableSemaphore, 0, 0 );

    //
    pthread_mutex_init(&pqContext[pqId].lock, NULL);

    // Start a reader thread.
    pthread_t   threadId;
    if(pthread_create( &threadId, NULL, packetProcessorThread, (void*)&pqContext[pqId])) {
        fprintf(stderr, "Error creating thread\n");
        exit(-1);
    }   
}


