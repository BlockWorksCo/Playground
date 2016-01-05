

#include <stdint.h>
#include <stdbool.h>
#include "CoreServices.h"


#define PANIC()             {while(true);}


//
//
//
extern volatile CoreServicesBridge      bridge;


//
// Get the Multiprocessor affinity register (core id).
//
uint32_t MPIDR()
{
    register uint32_t    mpidr;

    __asm__ volatile("mrc p15, 0, %0, c0, c0, 5\n\t" : : "r"(mpidr));    

    uint32_t coreID     = mpidr & 0x03;
    return coreID;
}



//
// Cause a reset request.
//
void CWRR()
{
    //
    // Cause a reset request.
    //
    uint32_t    cwrr    = 0x00000002;
    __asm__ volatile("mcr p14, 0, %0, c1, c4, 4\n\t" : : "r"(cwrr));
}



//
//
//
void CoreMain()
{

    //
    //
    //
    while(true)    
    {
        uint32_t coreID  = MPIDR();
        //register uint32_t    coreID;
        //__asm__ volatile("mrc p15, 0, %0, c0, c0, 5\n\t" : : "r"(coreID));    
        //coreID  = coreID & 0x03;

        bridge.heartBeats[coreID]++;
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



