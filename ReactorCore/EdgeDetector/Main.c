

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
void SendDoorBellToCore(uint32_t coreNumber, uint32_t mailboxNumber)
{
    //
    // Ensure that stores to Normal memory are visible to the
    // other CPUs before issuing the IPI.
    //
    //dsb();

    //
    // Cause the doorbell interrupt on the remote core.
    //
    uint32_t    thisCore    = MPIDR();
    uint32_t    address     = 0x40000080 + (0x10 * coreNumber) + (mailboxNumber*4);
    *((uint32_t*)address)   = 1 << thisCore;
}



//
//
//
void CoreMain()
{
    uint32_t coreID  = MPIDR();
    bridge.heartBeats[coreID]   = 0;

    //
    //
    //
    while(true)    
    {
        //register uint32_t    coreID;
        //__asm__ volatile("mrc p15, 0, %0, c0, c0, 5\n\t" : : "r"(coreID));    
        //coreID  = coreID & 0x03;

        bridge.heartBeats[coreID]++;

        if( (bridge.heartBeats[coreID] % 0x4ffff) == 0 )
        {
            //
            // Notify ControllerCore that we've started up.
            //
            SendDoorBellToCore(0, 2);            
        }
    }    
}




uint8_t     stack[1024];

void __attribute__ ( ( naked ) ) EntryPoint()
{
    //
    // Setup the stack.
    // TODO: Make this a different stack for each core (based on MPIDR).
    //
    uint32_t            stackPointer    = ((uint32_t)&stack[0]) + sizeof(stack);
    __asm__ volatile("MOV sp, %0\n\t" : : "r"(stackPointer));

    //
    // Notify ControllerCore that we've started up.
    //
    //SendDoorBellToCore(0, 2);

    //
    // Call the CoreMain.
    //
    CoreMain();

    //
    // Should never get here.
    //
    PANIC();
}



