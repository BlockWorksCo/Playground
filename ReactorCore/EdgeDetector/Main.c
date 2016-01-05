

#include <stdint.h>
#include <stdbool.h>
#include "CoreServices.h"


#define PANIC()             {while(true);}


//
//
//
extern volatile CoreServicesBridge      bridge;

//
//
//
void CoreMain()
{
    //
    // Cause a reset request.
    //
    uint32_t    cwrr    = 0x00000002;
    __asm__ volatile("mcr p14, 0, %0, c1, c4, 4\n\t" : : "r"(cwrr));

    //
    //
    //
    while(true)    
    {
        bridge.heartBeats[3]++;
    }    
}




uint8_t     stack[1024];

void __attribute__ ( ( naked ) ) EntryPoint()
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

    //
    // SHould never get here.
    //
    PANIC();
}



