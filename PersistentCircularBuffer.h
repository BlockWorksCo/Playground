//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//



//
// This buffer acts as its own journal and cache.
// - The last element is cached and can be updated before its flushed out.
// - The caching works because it is no longer a random-access device, only the last element needs
// to be cached.
// - On a shutdown, only the last element needs to be written to FLASH, its pre-erased, so no erasing
// needs to take place.
// - Erase errors are detected on eraseure by the FLASHDevice, pages are erased in-advance of the
// current position.
// - Still susceptible to program/write errors on a shutdown but nothing can prevent this and still be
//  deterministic.
// 
//
// Provides:
// - bad block management. deterministic over-provisioning.
// - wear levelling.
// - deterministic shutdown performance.
// - caching.
//


#ifndef __PERSISTENTCIRCULARBUFFER_H__
#define __PERSISTENTCIRCULARBUFFER_H__


#include "FLASHDevice.h"
#include <stdint.h>
#include <stdbool.h>


#define LAYOUT(page,numPages,elementSize)                                                           \
{                                                                                                   \
    .startPage                  = page,                                                             \
    .numberOfPages              = numPages,                                                         \
    .numberOfBytesPerElement    = elementSize,                                                      \
    .numberOfElementsPerPage    = PAGE_SIZE/(elementSize+sizeof(ElementMetadata)),                  \
    .numberOfElementsInTotal    = numPages * (PAGE_SIZE/(elementSize+sizeof(ElementMetadata))),     \
}

#define FULL_ELEMENT_SIZE(context)  (sizeof(ElementMetadata)+context->layout->numberOfBytesPerElement)

//
// Per-element metadata.
//
typedef struct
{
    uint32_t    sequenceNumber;
    uint32_t    crc;

} ElementMetadata;


//
// Where the buffer data is stored physically.
// Note: This is entirely static data.
//
typedef struct
{
    uint32_t    startPage;
    uint32_t    numberOfPages;
    uint32_t    numberOfBytesPerElement;
    uint32_t    numberOfElementsPerPage;
    uint32_t    numberOfElementsInTotal;
    
} PersistentCircularBufferLayout;


//
// Dynamic data related to the current state of the buffer.
//
typedef struct
{
    PersistentCircularBufferLayout* layout;
    
    uint32_t                        firstElement;
    uint32_t                        lastElement;
    uint32_t                        lastSequenceNumber;

    uint8_t                         writeBuffer[PAGE_SIZE];
    uint32_t                        writeBufferedPage;

    uint32_t                        currentPosition;

} PersistentCircularBufferContext;


void PersistentCircularBufferInitialise( PersistentCircularBufferContext* context, PersistentCircularBufferLayout* layout );

void PersistentCircularBufferUpdateLast( PersistentCircularBufferContext* context, uint8_t* data );
void PersistentCircularBufferAdd( PersistentCircularBufferContext* context, uint8_t* data );
void PersistentCircularBufferRemove( PersistentCircularBufferContext* context, uint8_t* data  );
void PersistentCircularBufferForEach( PersistentCircularBufferContext* context, bool (*fn)(uint32_t,uint8_t*) );
void PersistentCircularBufferFlush( PersistentCircularBufferContext* context );
uint32_t PersistentCircularBufferNumberOfElements( PersistentCircularBufferContext* context );
uint32_t PersistentCircularBufferCapacity( PersistentCircularBufferContext* context );

void PersistentCircularBufferEraseAll( PersistentCircularBufferContext* context );

void PersistentCircularBufferMoveToFirst( PersistentCircularBufferContext* context );
void PersistentCircularBufferMoveToLast( PersistentCircularBufferContext* context );
void PersistentCircularBufferBack( PersistentCircularBufferContext* context );
void PersistentCircularBufferForward( PersistentCircularBufferContext* context );
void PersistentCircularBufferPeek( PersistentCircularBufferContext* context, uint8_t* data );


void ShowState( PersistentCircularBufferContext* context );

#endif


