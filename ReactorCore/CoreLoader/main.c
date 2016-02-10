//
// https://github.com/embedded2014/elf-loader.git
//


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "loader.h"
#include "CoreServices.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>      /* ioctl */
#include <ctype.h>


#define PAGEMAP_LENGTH  8
#define PAGE_SHIFT      12

#define PERIPHERALS_RAM_BASE    (0x4000008c)

#define dsb(option) asm volatile ("dsb " #option : : : "memory")


#define DBG(...) printf("ELF: " __VA_ARGS__)
#define ERR(msg) do { perror("ELF: " msg); exit(-1); } while(0)
#define MSG(msg) puts("ELF: " msg)


ELFSymbol_t     exports[]   = {0};
ELFEnv_t        env         = { exports, 0 };

int             memFD       = -1;
uint8_t*        alloyRAM    = 0;
uint8_t*        topOfHeap   = 0;

int             peripheralsFD   = -1;
uint8_t*        peripherals     = 0;


//
//
//
void* do_alloc(size_t size, size_t align, ELFSecPerm_t perm, uint32_t* physicalAddress)
{
    //
    //
    //
    void*       block       = topOfHeap;
    uint32_t    blockSize   = ((size+(align/2))/align)*align;

    topOfHeap           += blockSize;
    *physicalAddress    = ((uint32_t)block - (uint32_t)alloyRAM) + ALLOY_RAM_BASE;

    printf("topOfHeap = %08x, block = %08x  physicalAddress=%08x alloyRAM=%08x\n", (uint32_t)topOfHeap, (uint32_t)block, (uint32_t)*physicalAddress, (uint32_t)alloyRAM );
    memset(block, 0, size);

    return block;
}




//
// Cause the specified core to start execution at the specified point in physical memory.
//
int GetBridgePhysicalAddress()
{
    return ALLOY_RAM_BASE;    
}

//
// Cause the specified core to start execution at the specified point in physical memory.
//
int TriggerCoreExecution(uint32_t coreID, uint32_t physicalAddress)
{
#if 0
    uint32_t*   startTrigger    = (uint32_t*)(peripherals + 0x8c + (coreID<<4));

    *startTrigger    = physicalAddress;
    dsb();
#else
    int     file_desc;

    file_desc = open("/dev/ReactorCoreServices", 0);

    if (file_desc < 0)
    {
        ERR("Can't open /dev/ReactorCoreServices");
    }

    int ret_val = ioctl(file_desc, IOCTL_SET_MSG, physicalAddress );

    if (ret_val < 0)
    {
        printf("ioctl failed:%d\n", ret_val);
        exit(-1);
    }
    
    close(file_desc);
#endif    
}


//
// Cause the specified core to start execution at the specified point in physical memory.
//
int SendMail(uint32_t coreNumber)
{
    int     file_desc;

    file_desc = open("/dev/ReactorCoreServices", 0);

    if (file_desc < 0)
    {
        ERR("Can't open /dev/ReactorCoreServices");
    }

    int ret_val = ioctl(file_desc, IOCTL_SEND_MAIL, coreNumber );

    if (ret_val < 0)
    {
        printf("ioctl failed:%d\n", ret_val);
        exit(-1);
    }
    
    close(file_desc);
}


extern volatile CoreServicesBridge     bridge;

//
//
//
void arch_jumpTo(entry_t entry)
{
    printf("EntryPoint @ %08x \n", (uint32_t)entry );

    //
    // Start the core executing.
    //
    TriggerCoreExecution( 3, (uint32_t)entry );

    //
    // Wait for completion.
    //
    while(true)
    {
        CoreServicesBridge*  b  = (CoreServicesBridge*)ALLOY_RAM_BASE;

        printf("[%08x %08x %08x %08x] ", b->heartBeats[0], b->heartBeats[1], b->heartBeats[2], b->heartBeats[3] );
        printf("[%08x %08x %08x %08x] \n", b->messageCounts[0], b->messageCounts[1], b->messageCounts[2], b->messageCounts[3] );

        b->coreMessages[2][0].type = CORE_MESSAGE_TEST;
        SendMail(2);

        sleep(1);
    }
}

//
//
//
int main(int argc, char* argv[])
{
    //
    //
    //
    memFD = open("/dev/mem", O_RDWR|O_SYNC);
    if(memFD < 0) 
    {
        perror("open");
        return -1;
    }

    alloyRAM = mmap( (void*)ALLOY_RAM_BASE, 1024*1024*8, PROT_READ|PROT_WRITE, MAP_SHARED , memFD, ALLOY_RAM_BASE );
    printf("Alloy RAM base = %08x\n", (uint32_t)alloyRAM);
    memset(alloyRAM, 0, 4096);
    topOfHeap   = alloyRAM + 4096;

    //
    //
    //
    peripheralsFD = open("/dev/mem", O_RDWR|O_SYNC);
    if(peripheralsFD < 0) 
    {
        perror("open");
        return -1;
    }

    peripherals = mmap( (void*)PERIPHERALS_RAM_BASE, 4096, PROT_READ|PROT_WRITE, MAP_SHARED , peripheralsFD, 0 );
    printf("peripherals RAM base = %08x\n", (uint32_t)peripherals);

    //
    //
    //
    exec_elf(argv[1], &env);
    puts("Done");

    return 0;
}


