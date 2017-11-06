


#include "SafeLib.h"
#include <stdio.h>
#include <stdlib.h>



void safeViolation()
{
    printf("!!! PROBLEMS! !!!");
    exit(-1);
}



#define NEW(typeName)   (typeName*)safeMalloc(sizeof(typeName))
#define BLAA(typeName)   printf("[%ld] %s[%c]\n",sizeof(typeName),#typeName, (#typeName)[1])

int main()
{
    BLAA(int);
    void*   b0  = safeMalloc(100);
    void*   b1  = safeMalloc(100);
    void*   b2  = safeMalloc(100);
    printf("b0=%p\n",b0);
    printf("b1=%p\n",b1);
    printf("b2=%p\n",b2);

    void*   low     = 0;
    void*   high    = 0;
    safeBoundsOfPointer( b0+50, &low, &high );
    printf("low=%p, high=%p\n", low, high);

    safeBoundsOfPointer( b1+50, &low, &high );
    printf("low=%p, high=%p\n", low, high);

    safeBoundsOfPointer( b2+50, &low, &high );
    printf("low=%p, high=%p\n", low, high);

    safeMemcpy( b0+10, b2+50, 25 );

    safeFree(b0);
    safeFree(b1);
    safeFree(b2);
}



