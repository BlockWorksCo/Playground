//
//
//

#include <stdint.h>
#include <stdbool.h>
#include "CoreServices.h"




#define dsb(option) asm volatile ("dsb " #option : : : "memory")
#define EI()        asm volatile ("cpsie i")
#define DI()        asm volatile ("cpsid i")

//
//
//
CoreServicesBridge*     bridge                      = (CoreServicesBridge*)BRIDGE_BASE;
volatile CoreMessage             message                     = {0};
volatile uint32_t                numberOfMessagesAvailable   = 0;


//
//
//
void PANIC()
{
    while(true);
}



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
    while(numberOfMessagesAvailable == 0)
    {
        //WFI();
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
CoreMessage* NextMessage()
{
    if(numberOfMessagesAvailable > 0)
    {
        return &message;
    }
    else
    {
        return 0;
    }
}

//
//
//
void ReleaseMessage(CoreMessage* msg)
{
    numberOfMessagesAvailable--;
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
void ProcessMessage(CoreMessage* msg)
{
    uint32_t    coreID  = MPIDR();
    bridge->messageCounts[coreID]++;

    //
    //
    //
    if(msg->type == CORE_MESSAGE_RESET)
    {
        //CWRR();
    }

    if(msg->type == CORE_MESSAGE_TEST)
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
    if(IsThereMailFromCore(0) == true)
    {
        message.type    = bridge->coreMessages[coreID][0].type;
        message.payload = bridge->coreMessages[coreID][0].payload;
        numberOfMessagesAvailable++;

        //ProcessMessage(&message);
        //ReleaseMessage(&message);
        
        ClearMailboxFromCore( 0 );
    }

    if(IsThereMailFromCore(1) == true)
    {
        message.type    = bridge->coreMessages[coreID][1].type;
        message.payload = bridge->coreMessages[coreID][1].payload;
        numberOfMessagesAvailable++;
        
        //ProcessMessage(&message);
        //ReleaseMessage(&message);
        
        ClearMailboxFromCore( 1 );
    }

    if(IsThereMailFromCore(2) == true)
    {
        message.type    = bridge->coreMessages[coreID][2].type;
        message.payload = bridge->coreMessages[coreID][2].payload;
        numberOfMessagesAvailable++;
        
        //ProcessMessage(&message);
        //ReleaseMessage(&message);
        
        ClearMailboxFromCore( 2 );
    }

    if(IsThereMailFromCore(3) == true)
    {
        message.type    = bridge->coreMessages[coreID][3].type;
        message.payload = bridge->coreMessages[coreID][3].payload;
        numberOfMessagesAvailable++;
        
        //ProcessMessage(&message);
        //ReleaseMessage(&message);

        ClearMailboxFromCore( 3 );
    }

}


//
//
//
void __attribute__ ( (naked, aligned(128) ) ) VectorTable()
{
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

        //
        //
        //
        CoreMessage*    msg     = NextMessage();
        if(msg != 0)
        {
            ProcessMessage( msg );

            ReleaseMessage( msg );
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
    // Setup the stack(s).
    //
    uint32_t   mpidr;

    //
    //
    //
    __asm__ volatile("mrc p15, 0, %0, c0, c0, 5\n\t" : "=r"(mpidr) ); 
    uint32_t    coreID  = mpidr&0x3;   

    //
    //
    //
    uint32_t            usrStackPointer    = ((uint32_t)&usrStack[coreID*STACK_SIZE]) + STACK_SIZE - 16;
    __asm__ volatile("MOV sp, %0\n\t" : : "r"(usrStackPointer));

    //
    //
    //
    uint32_t            irqStackPointer    = ((uint32_t)&irqStack[coreID*STACK_SIZE]) + STACK_SIZE - 16;
    __asm__ volatile("MSR     CPSR_c, 0xd2");
    __asm__ volatile("MOV sp, %0\n\t" : : "r"(irqStackPointer));
    __asm__ volatile("MSR     CPSR_c, 0xd3");

    //
    // Call the CoreMain.
    //
    CoreMain(coreID);

    //
    // Should never get here.
    //
    PANIC();
}



