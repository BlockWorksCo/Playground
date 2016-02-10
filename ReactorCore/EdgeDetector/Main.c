

#include <stdint.h>
#include <stdbool.h>
#include "CoreServices.h"


#define PANIC()             {while(true);}



#define dsb(option) asm volatile ("dsb " #option : : : "memory")
#define EI()        asm volatile ("cpsie i")
#define DI()        asm volatile ("cpsid i")

//
//
//
CoreServicesBridge*      bridge     = (CoreServicesBridge*)BRIDGE_BASE;

#if 0
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
#endif




//
// Get the Multiprocessor affinity register (core id).
//
uint32_t MPIDR()
{
    uint32_t    mpidr;

    __asm__ volatile("mrc p15, 0, %0, c0, c0, 5\n\t" : "=r"(mpidr));    

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
    //dsb();

    //
    // Cause the doorbell interrupt on the remote core.
    //
    //uint32_t    thisCore    = MPIDR();
    uint32_t    address     = 0x40000080 + (0x10 * coreNumber) + (mailboxNumber*4);
    *((uint32_t*)address)   = 1 << mailboxNumber;
}




#define STACK_SIZE              (1024)
#define NUMBER_OF_ALLOY_CORES   (4)
#define NUMBER_OF_VECTORS       (256)



//
//
//
void SetVectorTableAddress(uint32_t address)
{
    register uint32_t   temp    = address;
    asm volatile ("mcr p15, 0, %0, c12, c0,  0" : "=r" (temp));
}





//
//
//
void  __attribute__ ((interrupt ("IRQ"))) Handler()
{
    PANIC();
}


//
//
//
void SendMailboxFromCore(uint32_t toID)
{
    uint32_t    mailboxSetAddress;
    uint32_t    coreID  = MPIDR();

    mailboxSetAddress     = 0x40000080 + (0x10*toID);
    *(uint32_t*)mailboxSetAddress     = 1<<coreID;
}

//
//
//
void ClearMailboxFromCore(uint32_t fromID)
{
    uint32_t    mailboxClearAddress;
    uint32_t    coreID  = MPIDR();

    mailboxClearAddress     = 0x400000c0 + (0x10*coreID);
    *(uint32_t*)mailboxClearAddress     = 1<<fromID;
}

//
//
//
void EnableMailboxFromCore()
{
    uint32_t    coreID  = MPIDR();
    uint32_t    mailboxInterruptControlAddress  = 0x40000050+(coreID*4);
    uint32_t    currentSettings     = *(uint32_t*)mailboxInterruptControlAddress;

    currentSettings |= 0x0000000f;
    *(uint32_t*)mailboxInterruptControlAddress   = currentSettings;
}


//
//
//
void ProcessMailFromCore(uint32_t fromID)
{
    uint32_t    coreID  = MPIDR();

    if(bridge->coreMessages[coreID][fromID].type == CORE_MESSAGE_RESET)
    {
        uint32_t    mailboxClearAddress;

        bridge->coreMessages[coreID][fromID].type     = CORE_MESSAGE_NONE;
        bridge->messageCounts[coreID]++;

        //CWRR();
    }

    if(bridge->coreMessages[coreID][fromID].type == CORE_MESSAGE_TEST)
    {
    }

    if(coreID == 2)
    {
        SendMailboxFromCore( 3 );        
    }
    if(coreID == 3)
    {
        SendMailboxFromCore( 1 );        
    }
    //
    //
    //
    bridge->coreMessages[coreID][fromID].type     = CORE_MESSAGE_NONE;
    bridge->messageCounts[coreID]++;
}


//
//
//
void  __attribute__ ((interrupt ("IRQ"))) IRQHandler()
{
    ProcessMailFromCore( 0 );

    ClearMailboxFromCore( 0 );
    ClearMailboxFromCore( 1 );
    ClearMailboxFromCore( 2 );
    ClearMailboxFromCore( 3 );
}


//
//
//
void __attribute__ ( (naked, aligned(128) ) ) VectorTable()
{
#define ARM4_XRQ_RESET   0x00
#define ARM4_XRQ_UNDEF   0x01
#define ARM4_XRQ_SWINT   0x02
#define ARM4_XRQ_ABRTP   0x03
#define ARM4_XRQ_ABRTD   0x04
#define ARM4_XRQ_RESV1   0x05
#define ARM4_XRQ_IRQ     0x06
#define ARM4_XRQ_FIQ     0x07

//    asm volatile ("ldr pc, [pc, %0 ]" : : "i" (-16) );
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =IRQHandler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
    asm volatile ("ldr pc, =Handler");
}





//
//
//
void CoreMain(uint32_t coreID)
{
    //
    //
    //
    bridge->heartBeats[coreID]   = 0;

    //
    //
    //
    SetVectorTableAddress( (uint32_t)&VectorTable );

    //
    // Enable the malbox interrupt.
    //
    EnableMailboxFromCore();

    EI();

    //
    //
    //
    while(true)    
    {
        bridge->heartBeats[coreID]++;
        dsb();
        if( (bridge->heartBeats[coreID] % 0x4ffff) == 0 )
        {
            //
            // Notify ControllerCore that we've started up.
            //
            SendDoorBellToCore(0, coreID);            
        }
    }    
}



uint8_t     usrStack[NUMBER_OF_ALLOY_CORES*STACK_SIZE];
uint8_t     irqStack[NUMBER_OF_ALLOY_CORES*STACK_SIZE];


//
//
//
void __attribute__ ( ( naked ) ) EntryPoint()
{
    //
    // Setup the stack.
    // TODO: Make this a different stack for each core (based on MPIDR).
    //
    uint32_t   mpidr;

    __asm__ volatile("mrc p15, 0, %0, c0, c0, 5\n\t" : "=r"(mpidr) ); 
    uint32_t    coreID  = mpidr&0x3;   

    uint32_t            usrStackPointer    = ((uint32_t)&usrStack[coreID*STACK_SIZE]) + STACK_SIZE - 16;
    __asm__ volatile("MOV sp, %0\n\t" : : "r"(usrStackPointer));

    uint32_t            irqStackPointer    = ((uint32_t)&irqStack[coreID*STACK_SIZE]) + STACK_SIZE - 16;
    __asm__ volatile("MSR     CPSR_c, 0xd2");
    __asm__ volatile("MOV sp, %0\n\t" : : "r"(irqStackPointer));
    __asm__ volatile("MSR     CPSR_c, 0xd3");

    //
    //
    //
    //EnableMMU();
    //EnableCache();

    //
    // Call the CoreMain.
    //
    CoreMain(coreID);

    //
    // Should never get here.
    //
    PANIC();
}



