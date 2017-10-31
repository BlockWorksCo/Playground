


#include "SafeLib.h"
#include <stdio.h>



int main()
{
    void*   b0  = safeMalloc(100);
    printf("b0=%p\n",b0);

    void*   low     = 0;
    void*   high    = 0;
    safeBoundsOfPointer( b0+50, &low, &high );
    printf("low=%p, high=%p\n", low, high);

    safeFree(b0);
}



