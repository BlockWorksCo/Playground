


#include "SafeLib.h"
#include <stdio.h>



int main()
{
    void*   b0  = safeMalloc(100);
    printf("b0=%p\n",b0);
    safeFree(b0);
}



