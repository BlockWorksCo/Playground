

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
CoreServicesBridge*     bridge     = (CoreServicesBridge*)BRIDGE_BASE;
CoreMessage             message    = {0};
uint32_t                numberOfMessagesAvailable   = 0;


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
void WaitForMessage()
{
    while(true)
    {

    }
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
bool IsThereMailFromCore(uint32_t fromID)
{
    uint32_t    coreID              = MPIDR();
    uint32_t    mailboxAddress      = 0x400000c0 + (0x10*coreID);;
    uint32_t    mailboxSource       = *(uint32_t*)mailboxAddress;

    if( (mailboxSource&(1<<fromID)) != 0)
    {
        return true;
    }
    else
    {
        return false;
    }

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
    uint32_t    coreID  = MPIDR();

    //
    //
    //
    message.type    = bridge->coreMessages[coreID][0].type;
    message.payload = bridge->coreMessages[coreID][0].payload;
    numberOfMessagesAvailable++;

    ProcessMailFromCore( 0 );

    //
    //
    //
    if(IsThereMailFromCore(0) == true)
    {
        ClearMailboxFromCore( 0 );
    }

    if(IsThereMailFromCore(1) == true)
    {
        ClearMailboxFromCore( 1 );
    }

    if(IsThereMailFromCore(2) == true)
    {
        ClearMailboxFromCore( 2 );
    }

    if(IsThereMailFromCore(3) == true)
    {
        ClearMailboxFromCore( 3 );
    }
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



