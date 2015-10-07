


#include "CircularBuffer.h"
#include "BlockDevice.h"
#include <stdio.h>


//
//
//
typedef uint32_t    SequenceNumber;



//
// TODO: Make this a macro to create a static const context initalisation.
//
void CircularBufferContextInitialise(CircularBufferContext* context,  uint32_t offset, uint32_t containerSize, uint32_t elementSize)
{
    context->offset             = offset;
    context->containerSize      = containerSize;
    context->elementSize        = elementSize + sizeof(SequenceNumber);
    context->numberOfElements   = (containerSize / elementSize);
}


//
// TODO: Cache this value.
//
uint32_t FindLatestElementIndex( CircularBufferContext* context )
{
    SequenceNumber  latest          = 0;
    uint32_t        latestIndex     = 0;

    //
    //
    //
    for(uint32_t i=0; i<context->numberOfElements; i++)
    {
        SequenceNumber  sequenceNumber  = 0;
        BlockDeviceRead( context->offset+(context->elementSize*i), sizeof(SequenceNumber), (void*)&sequenceNumber );

        if((sequenceNumber >= latest) && (sequenceNumber != 0xffffffff))
        {
            //
            // This value has been set (its not 0xffffffff) and its larger than our previous one.
            //
            latest          = sequenceNumber;
            latestIndex     = i;
        }
    }

    return latestIndex;
}


//
// TODO: Cache this value.
//
uint32_t FindOldestElementIndex( CircularBufferContext* context )
{
    SequenceNumber  oldest          = 0xffffffff;
    uint32_t        oldestIndex     = 0;

    //
    //
    //
    for(uint32_t i=0; i<context->numberOfElements; i++)
    {
        SequenceNumber  sequenceNumber  = 0;
        BlockDeviceRead( context->offset+(context->elementSize*i), sizeof(SequenceNumber), (void*)&sequenceNumber );

        if((sequenceNumber <= oldest) && (sequenceNumber != 0xffffffff))
        {
            //
            // This value has been set (its not 0xffffffff) and its larger than our previous one.
            //
            oldest          = sequenceNumber;
            oldestIndex     = i;
        }
    }

    return oldestIndex;
}


//
//
//
SequenceNumber SequenceNumberOfElementAtIndex( CircularBufferContext* context, uint32_t index )
{
    SequenceNumber  sequenceNumber  = 0;

    BlockDeviceRead( context->offset+(context->elementSize*index), sizeof(SequenceNumber), (void*)&sequenceNumber );

    return sequenceNumber;    
}


//
// Walk one-past the latest element and write the new element.
//
void CircularBufferPut(CircularBufferContext* context,  void* element)
{
    uint32_t        latestIndex     = FindLatestElementIndex( context );
    uint32_t        newIndex        = (latestIndex + 1) % context->numberOfElements;
    SequenceNumber  sequenceNumber  = SequenceNumberOfElementAtIndex( context, latestIndex ) + 1;

    printf("latestIndex = %d\n", latestIndex);
    printf("newIndex = %d\n", newIndex);
    printf("sequenceNumber = %d\n\n", sequenceNumber);
    BlockDeviceWrite(   context->offset+(context->elementSize*newIndex),                          
                        sizeof(SequenceNumber),                     
                        (void*)&sequenceNumber );

    BlockDeviceWrite(   context->offset+(context->elementSize*newIndex)+sizeof(SequenceNumber),   
                        context->elementSize-sizeof(SequenceNumber), 
                        element );
}


//
// Clear the SequenceNumber of the oldest element to 0xffffffff and read the data.
//
void CircularBufferGet(CircularBufferContext* context,  void* element)
{
    uint32_t        oldestIndex     = FindOldestElementIndex( context );
    SequenceNumber  sequenceNumber  = 0xffffffff;

    printf("oldestIndex = %d\n", oldestIndex);

    BlockDeviceRead(    context->offset+(context->elementSize*oldestIndex)+sizeof(SequenceNumber),   
                        context->elementSize-sizeof(SequenceNumber), 
                        element );

    BlockDeviceWrite(   context->offset+(context->elementSize*oldestIndex),                          
                        sizeof(SequenceNumber),                     
                        (void*)&sequenceNumber );

}


