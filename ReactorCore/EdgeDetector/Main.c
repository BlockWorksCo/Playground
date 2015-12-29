

#include <stdint.h>
#include <stdbool.h>





void DoSomething()
{
    static volatile uint32_t     counter     = 0;
    while(true)    
    {
        counter++;
    }    
}


void EntryPoint()
{
    DoSomething();
}



