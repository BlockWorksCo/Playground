//
// gcc -finstrument-functions  -finstrument-functions-exclude-function-list=__cyg_profile_func_enter,__cyg_profile_func_exit    -o CheckedReturns CheckedReturns.c
//


#include <stdio.h>
#include <time.h>









void __cyg_profile_func_enter (void *func,  void *caller)
{
    printf("e %p %p %lu\n", func, caller, time(NULL) );
}

void __cyg_profile_func_exit (void *func, void *caller)
{
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



