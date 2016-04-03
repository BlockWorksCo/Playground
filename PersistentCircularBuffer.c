


#include "PersistentCircularBuffer.h"
#include <string.h>








//
//
//
void Read( PersistentCircularBufferContext* context, uint32_t offset, uint32_t numberOfBytes, uint8_t* data )
{
    uint32_t    page    = offset / PAGE_SIZE;

    if(page == context->writeBufferedPage)
    {
        uint32_t    pageOffset  = offset % PAGE_SIZE;
        memcpy( data, &context->writeBuffer[pageOffset], numberOfBytes );
    }
    else
    {
        FLASHDeviceRead( offset,   numberOfBytes,  data );
    }
}



//
//
//
void Write( PersistentCircularBufferContext* context, uint32_t offset, uint32_t numberOfBytes, uint8_t* data )
{
    uint32_t    page    = offset / PAGE_SIZE;

    if(page == context->writeBufferedPage)
    {
        uint32_t    pageOffset  = offset % PAGE_SIZE;
        memcpy( &context->writeBuffer[pageOffset], data, numberOfBytes );
    }
    else
    {
        FLASHDeviceWrite( offset,   numberOfBytes,  data );
    }
}



//
// Flush the write-buffered page to FLASH. The page is pre-erased as it is contrained
// to be at the end of the buffer (containing the 'last' element).
// This provides a quick, deterministic shutdown.
//
void PersistentCircularBufferFlush( PersistentCircularBufferContext* context )
{
    FLASHDeviceWrite( context->writeBufferedPage,   PAGE_SIZE,  &context->writeBuffer[0] );
}





//
// Read the metadata from each element in turn searching for the first and
// last elements.
//
void FindFirstAndLastElement( PersistentCircularBufferContext* context )
{
    uint32_t    lowestSequenceNumber    = (uint32_t)-1;
    uint32_t    highestSequenceNumber   = 0;
    uint8_t     elementNumber           = 0;

    for(uint32_t page=0; page<context->layout->numberOfPages; page++)
    {
        for(uint32_t i=0; i<context->layout->numberOfElementsPerPage; i++)
        {
            ElementMetadata     metadata;
            Read( context, (page*PAGE_SIZE)+(FULL_ELEMENT_SIZE(context)*i), sizeof(metadata), (uint8_t*)&metadata );

            if( metadata.sequenceNumber >= highestSequenceNumber )
            {
                highestSequenceNumber   = metadata.sequenceNumber;
                context->lastElement    = elementNumber;
            }

            if( metadata.sequenceNumber <= lowestSequenceNumber )
            {
                lowestSequenceNumber    = metadata.sequenceNumber;
                context->firstElement   = elementNumber;
            }

            elementNumber++;
        }
    }
}







void PersistentCircularBufferInitialise( PersistentCircularBufferContext* context, PersistentCircularBufferLayout* layout )
{
    context->layout     = layout;

    FindFirstAndLastElement( context );
}



//
// Take advantage of the guaranteed last-page write-buffer to allow quick updating of the 
// most recent element in the buffer.
//
void PersistentCircularBufferUpdateLast( PersistentCircularBufferContext* context, uint8_t* data )
{
    uint32_t    page            = context->lastElement / context->layout->numberOfElementsPerPage;
    uint32_t    elementIndex    = context->lastElement % context->layout->numberOfElementsPerPage;
    uint32_t    elementOffset   = (page * PAGE_SIZE) + (elementIndex * FULL_ELEMENT_SIZE(context));

    Write( context, elementOffset+sizeof(ElementMetadata),   context->layout->numberOfBytesPerElement,   data );
}



//
// Add an element to the buffer, this buffer is appended to the existing data.
// Only program, dont write here as pages are pre-erased.
//
void PersistentCircularBufferAdd( PersistentCircularBufferContext* context, uint8_t* data )
{
    uint32_t    page            = context->lastElement / context->layout->numberOfElementsPerPage;
    uint32_t    elementIndex    = context->lastElement % context->layout->numberOfElementsPerPage;
    uint32_t    elementOffset   = (page * PAGE_SIZE) + (elementIndex * FULL_ELEMENT_SIZE(context));

    context->lastSequenceNumber++;
    ElementMetadata     metadata    = 
    {
        .sequenceNumber     = context->lastSequenceNumber,
        .crc                = 0xa5a5,
    };

    Write( context, elementOffset,                    sizeof(metadata),                           (uint8_t*)&metadata );
    Write( context, elementOffset+sizeof(metadata),   context->layout->numberOfBytesPerElement,   data );
}



//
// Remove the last element from the buffer.
// - Erase pages as they become empty.
// - Update dynamic context to the new last entry.
// - Erase the metadata of the current last entry.
//
void PersistentCircularBufferRemove( PersistentCircularBufferContext* context )
{
    uint32_t    page            = context->firstElement / context->layout->numberOfElementsPerPage;
    uint32_t    elementIndex    = context->firstElement % context->layout->numberOfElementsPerPage;
    uint32_t    elementOffset   = (page * PAGE_SIZE) + (elementIndex * FULL_ELEMENT_SIZE(context));

    ElementMetadata     metadata    = 
    {
        .sequenceNumber     = (uint32_t)-1,
        .crc                = (uint32_t)-1,
    };

    Write( context, elementOffset,     sizeof(metadata),   (uint8_t*)&metadata );

    context->lastSequenceNumber++;

    //
    // Check to see if we can erase the page that contained the last element now that we've
    // moved on.
    // This is effectively the "pre-erase" so we can shutdown quickly by just doing a 'Write'
    // operation.
    //
}




//
// Call the given function for each element in the buffer in turn starting
// from first element and finishing at the last element.
// An index starting at 0 is gien to the function along with a temporary pointer to
// the element data.
//
void PersistentCircularBufferForEach( PersistentCircularBufferContext* context, bool (*fn)(uint32_t,uint8_t*) )
{
    uint32_t    i       = context->firstElement;

    while(i <= context->lastElement)
    {
        uint32_t    page            = i / context->layout->numberOfElementsPerPage;
        uint32_t    elementIndex    = i % context->layout->numberOfElementsPerPage;
        uint32_t    elementOffset   = (page * PAGE_SIZE) + (elementIndex * FULL_ELEMENT_SIZE(context));

        uint8_t             data[PAGE_SIZE];
        ElementMetadata     metadata;

        Read(context, elementOffset,                    sizeof(metadata),                           (uint8_t*)&metadata );
        Read(context, elementOffset+sizeof(metadata),   context->layout->numberOfBytesPerElement,   &data[0] );

        fn( i-context->firstElement, &data[0] );

        i++;
    }
}

