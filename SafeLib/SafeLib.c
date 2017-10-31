


#include "SafeLib.h"
#include <stdlib.h>

#define NUMBER_OF_ELEMENTS(a)   (sizeof(a)/sizeof(a[0]))



void* safeMalloc(size_t numberOfBytes)
{
    void*   newBlock    = malloc(numberOfBytes);
    safeBlockRegister(0, newBlock, numberOfBytes);
    return newBlock;
}

void safeFree(void* block)
{
    safeBlockUnregister(block);
    free(block);
}






SafeBlock   safeBlockList[128]  = {0};



SafeBlock* safeBlockOfPointer(void* pointer)
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(safeBlockList); i++)
    {
        if( (safeBlockList[i].block < pointer) && 
            (safeBlockList[i].block+safeBlockList[i].size > pointer) )
        {
            return &safeBlockList[i];
        }
    }

    return NULL;
}

void safeBlockRegister(Tag tag, void* block, size_t numberOfBytes)
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(safeBlockList); i++)
    {
        if( safeBlockList[i].block == 0 )
        {
            safeBlockList[i].block  = block;
            safeBlockList[i].size   = numberOfBytes;
            safeBlockList[i].tag    = tag;
            break;
        }
    }
}

void safeBlockUnregister(void* block)
{
    SafeBlock*  safeBlock   = safeBlockOfPointer(block);
   
    if( safeBlock != NULL )
    {
        safeBlock->block = 0;
    }
}




