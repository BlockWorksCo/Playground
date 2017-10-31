


#ifndef __SAFELIB_H__
#define __SAFELIB_H__


#include <stdint.h>
#include <stddef.h>


typedef uint32_t    Tag;

typedef struct
{
    void*   block;
    size_t  size;
    Tag     tag;

} SafeBlock;

void* safeMalloc(size_t numberOfBytes);
void safeFree(void* block);


void safeBlockRegister(Tag tag, void* block, size_t numberOfBytes);
SafeBlock* safeBlockOfPointer(void* pointer);
void safeBlockUnregister(void* block);

#endif



