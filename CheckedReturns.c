//
// gcc -finstrument-functions  -finstrument-functions-exclude-function-list=__cyg_profile_func_enter,__cyg_profile_func_exit    -o CheckedReturns CheckedReturns.c
//


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



typedef void*       address_t;

address_t       returnAddressStack[1024]    = {0};
uint32_t        returnAddressStackHead      = 0;





void __cyg_profile_func_enter (void *func,  void *caller)
{
    returnAddressStack[returnAddressStackHead]  = caller;
    returnAddressStackHead++;

    printf("e %p %p %lu\n", func, caller, time(NULL) );
}

void __cyg_profile_func_exit (void *func, void *caller)
{
    returnAddressStackHead--;
    if(caller != returnAddressStack[returnAddressStackHead] )
    {
        printf("<Mismatch!>\n");
        exit(-1);
    }

    printf("x %p %p %lu\n", func, caller, time(NULL));
}



void FuncC()
{
    printf("C\n");
}



void FuncB()
{
    printf("B\n");
    FuncC();
}





void FuncA()
{
    printf("A\n");
    FuncB();
}



int main()
{
    printf("CheckedReturns\n");
    FuncA();
}



