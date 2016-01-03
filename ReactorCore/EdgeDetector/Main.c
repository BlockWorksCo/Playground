

#include <stdint.h>
#include <stdbool.h>


//
//
//
uint32_t    Blaa[10]        = {0};
uint8_t     stack[1024];

void CoreMain()
{
    static volatile uint32_t     counter     = 0;
    while(true)    
    {
        Blaa[0]++;
        counter++;
    }    
}

//uint8_t      stack[1024*1];

void EntryPoint()
{
    //
    // Setup the stack.
    //
    uint32_t            stackPointer    = ((uint32_t)&stack[0]) + sizeof(stack);
    __asm__ volatile("MOV sp, %0\n\t" : : "r"(stackPointer));

    //
    // Call the CoreMain.
    //
    CoreMain();
}



