//
// gcc -finstrument-functions  -finstrument-functions-exclude-function-list=__cyg_profile_func_enter,__cyg_profile_func_exit,PushToAddressStack,PopFromAddressStack    -o CheckedReturns CheckedReturns.c
//


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



#define NUMBER_OF_ELEMENTS(a)       (sizeof(a)/sizeof(a[0]))

typedef void*       address_t;

address_t       returnAddressStack[1024]    = {0};
uint32_t        returnAddressStackHead      = 0;



//
// - Use MPU to make page writable.
// - Push address.
// - Use MPU to make page inaccessible.
//
void PushToAddressStack( address_t address )
{
    //
    // Push
    //
    returnAddressStack[returnAddressStackHead]  = address;
    returnAddressStackHead++;

    //
    // Check.
    //
    if( returnAddressStackHead >= NUMBER_OF_ELEMENTS(returnAddressStack) )
    {
        printf("<Underflow of returnStack>\n" );
        exit(-1);
    }
}

//
// - Use MPU to make page writable.
// - Pop address.
// - Use MPU to make page inaccessible.
//
address_t PopFromAddressStack()
{
    //
    // Check.
    //
    returnAddressStackHead--;

    if( returnAddressStackHead < 0 )
    {
        printf("<Underflow of returnStack>\n" );
        exit(-1);
    }

    //
    // Pop.
    //
    address_t   address = returnAddressStack[returnAddressStackHead];
    return address;
}




//
// - Push address of caller.
//
void __cyg_profile_func_enter (void *func,  void *caller)
{
    printf("e %p %p %lu\n", func, caller, time(NULL) );

    PushToAddressStack( caller );
}

//
// - Pop address of caller.
// - If caller doesn't match saved caller, PANIC.
//
void __cyg_profile_func_exit (void *func, void *caller)
{
    printf("x %p %p %lu\n", func, caller, time(NULL));

    //
    // Check for match against duplicate.
    //
    address_t   realAddress     = PopFromAddressStack();
    if(caller != realAddress )
    {
        printf("<Mismatch! %p != %p>\n", caller, realAddress );
        exit(-1);
    }
}






void FuncBAD()
{
    address_t    blaa[1];

    printf("BAD\n");
    blaa[3]    = (address_t)&FuncBAD;
}





void FuncC()
{
    address_t    blaa[1];

    printf("C\n");
    blaa[3]    = (address_t)&FuncBAD;
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



