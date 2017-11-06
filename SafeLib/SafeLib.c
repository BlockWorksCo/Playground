


#include "SafeLib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
        if( (safeBlockList[i].block <= pointer) && 
            (safeBlockList[i].block+safeBlockList[i].size > pointer) )
        {
            return &safeBlockList[i];
        }
    }

    printf("BAD [%p]\n",pointer);
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
        safeBlock->block    = 0;
        safeBlock->size     = 0;
        safeBlock->tag      = 0;
    }
}


bool safeBoundsOfPointer(void* pointer, void** low, void** high)
{
    SafeBlock*  safeBlock   = safeBlockOfPointer(pointer);

    if( safeBlock != NULL )
    {
        *low    = safeBlock->block;
        *high   = (*low) + safeBlock->size;
        return true;
    }
    else
    {
        return false;
    }
}


bool safeBlockCheck(void* blockA, void* blockB)
{
    void*   aLow        = 0;
    void*   aHigh       = 0;
    void*   bLow        = 0;
    void*   bHigh       = 0;
    bool    successFlag = true;

    successFlag &= safeBoundsOfPointer(blockA, &aLow, &aHigh);
    successFlag &= safeBoundsOfPointer(blockB, &bLow, &bHigh);
    
    if( (aLow==bLow) && (aHigh==bHigh) &&
        (successFlag==true) )
    {
        return true;
    }
    else
    {
        return false;
    }
}





void safeMemcpy(void* dst, void* src, size_t numberOfBytes)
{
    void*   dstLow      = 0;
    void*   dstHigh     = 0;
    void*   srcLow      = 0;
    void*   srcHigh     = 0;
    bool    successFlag = true;

    successFlag   &= safeBoundsOfPointer(dst, &dstLow, &dstHigh);
    successFlag   &= safeBoundsOfPointer(src, &srcLow, &srcHigh);

    successFlag   &= safeBlockCheck(dst, dst+numberOfBytes);
    successFlag   &= safeBlockCheck(src, src+numberOfBytes);

    if( successFlag == true )
    {
        memcpy( dst, src, numberOfBytes );
    }
    else
    {
        safeViolation();
    }
}


void safeStrcpy(char* dst, char* src)
{
    void*   dstLow      = 0;
    void*   dstHigh     = 0;
    void*   srcLow      = 0;
    void*   srcHigh     = 0;
    bool    successFlag = true;

    successFlag   &= safeBoundsOfPointer(dst, &dstLow, &dstHigh);
    successFlag   &= safeBoundsOfPointer(src, &srcLow, &srcHigh);

    successFlag   &= safeBlockCheck(dst, dst+strlen(src));
    successFlag   &= safeBlockCheck(src, src+strlen(src));

    if( successFlag == true )
    {
        strcpy( dst, src);
    }
    else
    {
        safeViolation();
    }
}

