

#include <stdint.h>
#include <stdbool.h>
#include "CoreServices.h"


#define PANIC()             {while(true);}



#define dsb(option) __asm__ __volatile__ ("dsb " #option : : : "memory")

//
//
//
extern volatile CoreServicesBridge      bridge;


void EnableCache()
{
#define ARM_AUX_CONTROL_SMP   (1 << 6)
#define ARM_CONTROL_BRANCH_PREDICTION      (1 << 11)
#define ARM_CONTROL_L1_INSTRUCTION_CACHE   (1 << 12)

   register uint32_t nAuxControl;
   asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (nAuxControl));
   nAuxControl |= ARM_AUX_CONTROL_SMP;
   asm volatile ("mcr p15, 0, %0, c1, c0,  1" : : "r" (nAuxControl));   // SMP bit must be set according to ARM TRM

   register uint32_t nControl;
   asm volatile ("mrc p15, 0, %0, c1, c0,  0" : "=r" (nControl));
   nControl |= ARM_CONTROL_BRANCH_PREDICTION | ARM_CONTROL_L1_INSTRUCTION_CACHE;
   asm volatile ("mcr p15, 0, %0, c1, c0,  0" : : "r" (nControl) : "memory");    
}


void EnableMMU (void)     // not fully optimized
{
    static volatile __attribute__ ((aligned (0x4000))) uint32_t PageTable[4096];

    uint32_t base;
    for (base = 0; base < 1024-16; base++)
    {
        // section descriptor (1 MB)
        // outer and inner write back, write allocate, not shareable (fast but unsafe)
        //PageTable[base] = base << 20 | 0x0140E;
        // outer and inner write through, no write allocate, shareable (safe but slower)
        PageTable[base] = base << 20 | 0x1040A;
    }

    for (; base < 4096; base++)
    {
        // shared device, never execute
        PageTable[base] = base << 20 | 0x10416;
    }

    // set SMP bit in ACTLR
    uint32_t auxctrl;
    asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (auxctrl));
    auxctrl |= 1 << 6;
    asm volatile ("mcr p15, 0, %0, c1, c0,  1" :: "r" (auxctrl));

    // set domain 0 to client
    asm volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (1));

    // always use TTBR0
    asm volatile ("mcr p15, 0, %0, c2, c0, 2" :: "r" (0));

    // set TTBR0 (page table walk inner and outer non-cacheable, non-shareable memory)
    asm volatile ("mcr p15, 0, %0, c2, c0, 0" :: "r" (0 | (unsigned) &PageTable));

    asm volatile ("isb" ::: "memory");

    // enable MMU, caches and branch prediction in SCTLR
    register uint32_t mode;
    asm volatile ("mrc p15, 0, %0, c1, c0, 0" : "=r" (mode));
    mode |= 0x1805;
    asm volatile ("mcr p15, 0, %0, c1, c0, 0" :: "r" (mode) : "memory");
}

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
    register uint32_t    cwrr    = 0x00000002;
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
    dsb();

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
        dsb();

        if( (bridge.heartBeats[coreID] % 0x4ffff) == 0 )
        {
            //
            // Notify ControllerCore that we've started up.
            //
            SendDoorBellToCore(0, coreID);            
        }
    }    
}


#define STACK_SIZE              (1024)
#define NUMBER_OF_ALLOY_CORES   (3)


uint8_t     stack[NUMBER_OF_ALLOY_CORES*STACK_SIZE];

void __attribute__ ( ( naked ) ) EntryPoint()
{
    //
    // Setup the stack.
    // TODO: Make this a different stack for each core (based on MPIDR).
    //
    register uint32_t   mpidr;

    __asm__ volatile("mrc p15, 0, %0, c0, c0, 5\n\t" : : "r"(mpidr));    

    uint32_t coreID     = mpidr & 0x03;
    register uint32_t            stackPointer    = ((uint32_t)&stack[coreID*STACK_SIZE]) + STACK_SIZE;
    __asm__ volatile("MOV sp, %0\n\t" : : "r"(stackPointer));

    //
    //
    //
    //EnableMMU();
    EnableCache();

    //
    // Call the CoreMain.
    //
    CoreMain();

    //
    // Should never get here.
    //
    PANIC();
}



