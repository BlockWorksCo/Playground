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
    printf("e %p %p %lu\n", func, caller, time(NULL) );

    returnAddressStack[returnAddressStackHead]  = caller;
    returnAddressStackHead++;

}

void __cyg_profile_func_exit (void *func, void *caller)
{
    printf("x %p %p %lu\n", func, caller, time(NULL));

    returnAddressStackHead--;
    if(caller != returnAddressStack[returnAddressStackHead] )
    {
        printf("<Mismatch! %p != %p>\n", caller, returnAddressStack[returnAddressStackHead] );
        exit(-1);
    }

}






void FuncBAD()
{
    uint32_t    blaa[1];

    printf("BAD\n");
    blaa[6]    = (address_t)&FuncBAD;
}





void FuncC()
{
    uint32_t    blaa[1];

    printf("C\n");
    blaa[6]    = (address_t)&FuncBAD;
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



