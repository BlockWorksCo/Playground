


#include "SafeLib.h"
#include <stdio.h>



int main()
{
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

    safeFree(b0);
    safeFree(b1);
    safeFree(b2);
}



