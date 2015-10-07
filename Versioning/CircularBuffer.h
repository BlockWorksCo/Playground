



#ifndef __CIRCULARBUFFER_H__
#define __CIRCULARBUFFER_H__



#include <stdint.h>
#include <stdbool.h>


//
//
//
typedef struct
{
    uint32_t    offset;
    uint32_t    containerSize;
    uint32_t    elementSize;
    uint32_t    numberOfElements;

} CircularBufferContext;


//
//
//
void CircularBufferContextInitialise(CircularBufferContext* context,  uint32_t offset, uint32_t containerSize, uint32_t elementSize);
void CircularBufferPut(CircularBufferContext* context,  void* element);
void CircularBufferGet(CircularBufferContext* context,  void* element);


#endif



