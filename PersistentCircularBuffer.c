//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//



#include "PersistentCircularBuffer.h"
#include <string.h>
#include <stdio.h>




void ShowState( PersistentCircularBufferContext* context )
{
    printf("\n\n");

    printf("firstElement = %d\n", context->firstElement );
    printf("lastElement = %d\n", context->lastElement );
    printf("lastSequenceNumber = %d\n", context->lastSequenceNumber );
    printf("writeBufferedPage = %d\n", context->writeBufferedPage );

    printf("startPage = %d\n", context->layout->startPage );
    printf("numberOfPages = %d\n", context->layout->numberOfPages );
    printf("numberOfBytesPerElement = %d\n", context->layout->numberOfBytesPerElement );
    printf("numberOfElementsPerPage = %d\n", context->layout->numberOfElementsPerPage );
    printf("numberOfElementsInTotal = %d\n", context->layout->numberOfElementsInTotal );

    printf("\n\n");
}


//
//
//
void Read( PersistentCircularBufferContext* context, uint32_t offset, uint32_t numberOfBytes, uint8_t* data )
{
    uint32_t    page    = offset / PAGE_SIZE;

    if(page == context->writeBufferedPage)
    {
        uint32_t    pageOffset  = offset % PAGE_SIZE;

        //printf("-- Loading cache from page %d\n", page);
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
        //printf("-- Saving cache to page %d\n", page);
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
    FLASHDeviceWrite( context->writeBufferedPage*PAGE_SIZE,   PAGE_SIZE,  &context->writeBuffer[0] );
}


//
//
//
void PersistentCircularBufferMoveWriteBuffer( PersistentCircularBufferContext* context, uint32_t newPage )
{
    FLASHDeviceWrite( context->writeBufferedPage*PAGE_SIZE,   PAGE_SIZE,  &context->writeBuffer[0] );
    context->writeBufferedPage  = newPage;
    FLASHDeviceRead( context->writeBufferedPage*PAGE_SIZE,   PAGE_SIZE,  &context->writeBuffer[0] );
    FLASHDeviceErasePage( context->writeBufferedPage );
}





//
// Read the metadata from each element in turn searching for the first and
// last elements.
//
void FindFirstAndLastElement( PersistentCircularBufferContext* context )
{
    uint32_t    lowestSequenceNumber    = (uint32_t)-1;
    uint32_t    highestSequenceNumber   = 0;
    uint32_t    elementNumber           = 0;

    context->firstElement   = 0;
    context->lastElement    = 0;
    for(uint32_t page=0; page<context->layout->numberOfPages; page++)
    {
        for(uint32_t i=0; i<context->layout->numberOfElementsPerPage; i++)
        {
            ElementMetadata     metadata;
            FLASHDeviceRead( (page*PAGE_SIZE)+(FULL_ELEMENT_SIZE(context)*i), sizeof(metadata), (uint8_t*)&metadata );

            //printf("%d, %d,%d) %d\n", elementNumber, page,i, metadata.sequenceNumber );

            if( metadata.sequenceNumber >= highestSequenceNumber )
            {
                //printf("<high>\n");
                highestSequenceNumber   = metadata.sequenceNumber;
                context->lastElement    = elementNumber;
            }

            if( metadata.sequenceNumber <= lowestSequenceNumber )
            {
                //printf("<low>\n");
                lowestSequenceNumber    = metadata.sequenceNumber;
                context->firstElement   = elementNumber;
            }

            elementNumber++;
        }
    }
}



//
//
//
uint32_t OffsetOfElement( PersistentCircularBufferContext* context, uint32_t elementNumber)
{
    uint32_t    page            = elementNumber / context->layout->numberOfElementsPerPage;
    uint32_t    elementIndex    = elementNumber % context->layout->numberOfElementsPerPage;
    uint32_t    elementOffset   = (page * PAGE_SIZE) + (elementIndex * FULL_ELEMENT_SIZE(context));

    return elementOffset;    
}


//
// Setup the dynamic context data for the circular buffer.
// - Finds the first and last element
//
void PersistentCircularBufferInitialise( PersistentCircularBufferContext* context, PersistentCircularBufferLayout* layout )
{
    context->layout     = layout;

    FindFirstAndLastElement( context );

    //
    // TODO: Load up the write buffer.
    //
    context->writeBufferedPage  = context->lastElement / context->layout->numberOfElementsPerPage;
    FLASHDeviceRead( context->writeBufferedPage*PAGE_SIZE,   PAGE_SIZE,  &context->writeBuffer[0] );
    FLASHDeviceErasePage( context->writeBufferedPage );
    printf("Preloaded page %d %d\n", context->writeBufferedPage, context->writeBufferedPage*PAGE_SIZE);

    //
    // Set the current position to element 0.
    //  
    context->currentPosition        = 0;
}



//
// Take advantage of the guaranteed last-page write-buffer to allow quick updating of the 
// most recent element in the buffer.
//
void PersistentCircularBufferUpdateLast( PersistentCircularBufferContext* context, uint8_t* data )
{
    uint32_t    elementOffset   = OffsetOfElement(context, context->lastElement);

    Write( context, elementOffset+sizeof(ElementMetadata),   context->layout->numberOfBytesPerElement,   data );
}



//
// Add an element to the buffer, this buffer is appended to the existing data.
// Only program, dont write here as pages are pre-erased.
//
void PersistentCircularBufferAdd( PersistentCircularBufferContext* context, uint8_t* data )
{
    uint32_t    elementOffset   = OffsetOfElement(context, context->lastElement);

    printf("Adding element = %d\n", elementOffset );

    //
    // Make a new element with an increasing sequence number (so we can identify the
    // first and last after a reset).
    //
    context->lastSequenceNumber++;
    ElementMetadata     metadata    = 
    {
        .sequenceNumber     = context->lastSequenceNumber,
        .crc                = 0xa5a5,
    };

    //
    // Move the last-element pointer around the buffer in a circular manner, overwriting
    // old data when needed.
    //
    context->lastElement   = (context->lastElement + 1) % context->layout->numberOfElementsInTotal;

    Write( context, elementOffset,                    sizeof(metadata),                           (uint8_t*)&metadata );
    Write( context, elementOffset+sizeof(metadata),   context->layout->numberOfBytesPerElement,   data );

    //
    // If the write-buffered page has changed (to accomodate the new lastElement) then flush the
    // write-buffered page and read the new one.
    //
    if( (context->lastElement/context->layout->numberOfElementsPerPage) != context->writeBufferedPage )
    {
        PersistentCircularBufferMoveWriteBuffer(context, context->lastElement / context->layout->numberOfElementsPerPage );
    }
}



//
// Remove the last element from the buffer.
// - Erase pages as they become empty.
// - Update dynamic context to the new last entry.
// - Erase the metadata of the current last entry.
//
void PersistentCircularBufferRemove( PersistentCircularBufferContext* context, uint8_t* data )
{
    uint32_t    elementOffset   = OffsetOfElement(context, context->firstElement);

    printf("Removing element %d\n", elementOffset );

    //
    // Read the data 
    //
    Read( context, elementOffset+sizeof(ElementMetadata),   context->layout->numberOfBytesPerElement,   data );

    //
    // Update the metadata of the current last-element to remove it from the buffer.
    //
    ElementMetadata     metadata    = 
    {
        .sequenceNumber     = (uint32_t)-1,
        .crc                = (uint32_t)-1,
    };

    Write( context, elementOffset,     sizeof(metadata),   (uint8_t*)&metadata );

    //
    // Move the first element index around the buffer.
    //
    context->firstElement   = (context->firstElement + 1) % context->layout->numberOfElementsInTotal;

    //
    // Check to see if we can erase the page that contained the last element now that we've
    // moved on.
    // This is effectively the "pre-erase" so we can shutdown quickly by just doing a 'Write'
    // operation.
    //
    //if( (context->firstElement/PAGE_SIZE) != page )
    {
        //
        // New first-page != old first-page, so erase old first-page.
        //
        //FLASHDeviceErasePage( page/PAGE_SIZE );
    }
}



//
//
//
void PersistentCircularBufferEraseAll( PersistentCircularBufferContext* context )
{
    for(uint32_t i=0; i<context->layout->numberOfPages; i++)
    {
        FLASHDeviceErasePage(context->layout->startPage + i);
    }
}













//
// Navigation, search & status operations (read-only).
//




//
// Call the given function for each element in the buffer in turn starting
// from first element and finishing at the last element.
// An index starting at 0 is gien to the function along with a temporary pointer to
// the element data.
//
void PersistentCircularBufferForEach( PersistentCircularBufferContext* context, bool (*fn)(uint32_t,uint8_t*) )
{
    uint32_t    i       = context->firstElement;

    while(i != context->lastElement)
    {
        uint32_t            elementOffset   = OffsetOfElement( context, i );
        uint8_t             data[PAGE_SIZE];
        ElementMetadata     metadata;

        Read(context, elementOffset,                    sizeof(metadata),                           (uint8_t*)&metadata );
        Read(context, elementOffset+sizeof(metadata),   context->layout->numberOfBytesPerElement,   &data[0] );

        fn( metadata.sequenceNumber, &data[0] );

        i   = (i+1) % context->layout->numberOfElementsInTotal;
    }
}


uint32_t PersistentCircularBufferNumberOfElements( PersistentCircularBufferContext* context )
{
    uint32_t    i       = context->firstElement;
    uint32_t    count   = 0;

    while(i != context->lastElement)
    {
        uint32_t            elementOffset   = OffsetOfElement( context, i );
        ElementMetadata     metadata;

        Read(context, elementOffset,                    sizeof(metadata),                           (uint8_t*)&metadata );

        count++;
        i   = (i+1) % context->layout->numberOfElementsInTotal;
    }

    return count;
}


uint32_t PersistentCircularBufferCapacity( PersistentCircularBufferContext* context )
{
    return context->layout->numberOfElementsInTotal;
}


void PersistentCircularBufferMoveToFirst( PersistentCircularBufferContext* context )
{
    context->currentPosition    = context->firstElement;
}


void PersistentCircularBufferMoveToLast( PersistentCircularBufferContext* context )
{
    context->currentPosition    = context->lastElement;
}


void PersistentCircularBufferBack( PersistentCircularBufferContext* context )
{
    context->currentPosition    = (context->currentPosition - 1) % context->layout->numberOfElementsInTotal;
}


void PersistentCircularBufferForward( PersistentCircularBufferContext* context )
{
    context->currentPosition    = (context->currentPosition + 1) % context->layout->numberOfElementsInTotal;
}


void PersistentCircularBufferPeek( PersistentCircularBufferContext* context, uint8_t* data )
{
    uint32_t            elementOffset   = OffsetOfElement( context, context->currentPosition );
    ElementMetadata     metadata;

    Read(context, elementOffset,                    sizeof(metadata),                           (uint8_t*)&metadata );
    Read(context, elementOffset+sizeof(metadata),   context->layout->numberOfBytesPerElement,   &data[0] );
}






