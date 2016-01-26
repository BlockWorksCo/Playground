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



#define DBG(...) printf("ELF: " __VA_ARGS__)
#define ERR(msg) do { perror("ELF: " msg); exit(-1); } while(0)
#define MSG(msg) puts("ELF: " msg)


ELFSymbol_t     exports[]   = {0};
ELFEnv_t        env         = { exports, 0 };


//
//
//
uint32_t get_page_frame_number_of_address(void* addr)
{
    uint32_t    returnValue     = (uint32_t)-1;

    // Open the pagemap file for the current process
    FILE* pagemap = fopen("/proc/self/pagemap", "rb");
    if(pagemap != NULL)
    {
        // Seek to the page that the buffer is on
        unsigned long offset = (unsigned long)addr / getpagesize() * PAGEMAP_LENGTH;

        if(fseek(pagemap, (unsigned long)offset, SEEK_SET) != 0)
        {
            fprintf(stderr, "Failed to seek pagemap to proper location\n");
            exit(1);
        }

        // The page frame number is in bits 0-54 so read the first 7 bytes and clear the 55th bit
        uint32_t    page_frame_number = 0;
        fread(&page_frame_number, 1, PAGEMAP_LENGTH - 1, pagemap);
        page_frame_number &= 0x7FFFFFFFFFFFFF;
        fclose(pagemap);

        returnValue     = page_frame_number;
    }
    else
    {
        perror("Couldn't open pagemap.\n");
        exit(-1);
    }

    return returnValue;
}


//
//
//
uint32_t PhysicalAddressOf(uint32_t virtualAddress)
{
    uint32_t pageFrameNumber        = get_page_frame_number_of_address( (void*)virtualAddress );
    const uint32_t  pageSize        = 1 << PAGE_SHIFT;
    uint32_t offsetIntoPage         = virtualAddress % pageSize;
    uint32_t physicalAddressOfPage  = pageFrameNumber << PAGE_SHIFT;
    uint32_t physicalAddress        = physicalAddressOfPage + offsetIntoPage;

    return physicalAddressOfPage;
}


//
//
//
void* do_alloc(size_t size, size_t align, ELFSecPerm_t perm, uint32_t* physicalAddress)
{
    (void) perm;
    //
    // Allocate the block, aligned on page size..
    //
    void*   block   = memalign(32768, size);
    //
    // Set the attributes to allow execution.
    //
    int     r       = mprotect(block, size, PROT_EXEC | PROT_READ | PROT_WRITE);

    if(r != 0)
    {
        perror("mprotect failed.");
        exit(-1);
    }

    //
    // Lock it in physical RAM.
    //
    r   = mlock(block, size);

    if(r == -1)
    {
        perror("mlock failed.");
        exit(-1);
    }

    //
    // Get the physical address of this virtual block address.
    //
    uint32_t    pageFrameNumber     = get_page_frame_number_of_address( block );
    *physicalAddress     = (pageFrameNumber << PAGE_SHIFT) + 0;

    printf("physical address = %08x virtual address = %08x size=%d\n", *physicalAddress, (uint32_t)block, size );

    return block;
}


//
//
//
static inline void dcache_clean(void)
 {
     const int zero = 0;
     /* clean entire D cache -> push to external memory. */
     __asm volatile ("1: mrc p15, 0, r15, c7, c10, 3\n"
                     " bne 1b\n" ::: "cc");
     /* drain the write buffer */
    __asm volatile ("mcr 15, 0, %0, c7, c10, 4"::"r" (zero));
 }




//
// Cause the specified core to start execution at the specified point in physical memory.
//
void GetBridgeData( CoreServicesBridge* bridge )
{
    int     file_desc;

    file_desc = open("/dev/ReactorCoreServices", 0);

    if (file_desc < 0)
    {
        ERR("Can't open /dev/ReactorCoreServices");
    }

    int ret_val = ioctl(file_desc, IOCTL_GET_MSG, bridge );

    if (ret_val < 0)
    {
        printf("ioctl failed:%d\n", ret_val);
        exit(-1);
    }
    
    close(file_desc);
}



//
// Cause the specified core to start execution at the specified point in physical memory.
//
void Allocate( uint32_t numberOfBytes )
{
    int     file_desc;

    file_desc = open("/dev/ReactorCoreServices", 0);

    if (file_desc < 0)
    {
        ERR("Can't open /dev/ReactorCoreServices");
    }

    AllocateRequest     request;
    request.size            = numberOfBytes;
    request.virtualAddress  = 0;
    request.physicalAddress = 0;
    int ret_val = ioctl(file_desc, IOCTL_ALLOCATE_BLOCK, &request );

    if (ret_val < 0)
    {
        printf("ioctl failed:%d\n", ret_val);
        exit(-1);
    }
    
    close(file_desc);

    printf("virtualAddress = %08x\n", request.virtualAddress);
    printf("physicalAddress = %08x\n", request.physicalAddress);
}


//
// Cause the specified core to start execution at the specified point in physical memory.
//
int GetBridgePhysicalAddress()
{
    int     file_desc;

    file_desc = open("/dev/ReactorCoreServices", 0);

    if (file_desc < 0)
    {
        ERR("Can't open /dev/ReactorCoreServices");
    }

    uint32_t physicalAddress    = 0;
    int ret_val = ioctl(file_desc, IOCTL_GET_BRIDGE_ADDRESS, &physicalAddress );

    if (ret_val < 0)
    {
        printf("ioctl failed:%d\n", ret_val);
        exit(-1);
    }
    
    close(file_desc);

    return physicalAddress;
}

//
// Cause the specified core to start execution at the specified point in physical memory.
//
int TriggerCoreExecution(uint32_t coreNumber, uint32_t physicalAddress)
{
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
        CoreServicesBridge  b;
        GetBridgeData( &b );

        uint32_t    temp    = PhysicalAddressOf( (uint32_t)&bridge );
        printf("<%08x> %08x %08x %08x %08x\n", temp, b.heartBeats[0], b.heartBeats[1], b.heartBeats[2], b.heartBeats[3] );

        //SendMail(2);

        sleep(1);
    }
}




#define PAGE_SIZE 4096

void Test()
{
    int configfd;
    configfd = open("/dev/ReactorCoreServices", O_RDWR);
    if(configfd < 0) 
    {
        perror("open");
        return -1;
    }

    char* address = NULL;
    address = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, 0);
    if (address == MAP_FAILED) 
    {
        perror("mmap");
        return -1;
    }

    printf("initial message: %s\n", address);
    memcpy(address + 11 , "*user*", 6);
    printf("changed message: %s\n", address);
    close(configfd);    
    
    return 0;
}




#define dsb(option) asm volatile ("dsb " #option : : : "memory")

//
// examine /proc/maps
//
void Test2()
{
    int fd;
    fd = open("/dev/mem", O_RDWR);
    if(fd < 0) 
    {
        perror("open");
        return -1;
    }

    uint8_t*    ptr = mmap(ALLOY_RAM_BASE, getpagesize(), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED , fd, ALLOY_RAM_BASE );
    printf("base = %08x\n", ptr);

    for(uint32_t i=0; i<16; i++)
    {
        ptr[i] = i;
        printf("%02x ", ptr[i]);
    }

    for(uint32_t i=0; i<64; i++)
    {
        printf("%02x ", ptr[i]);
    }

    printf("\n");

    dsb(sy);
}



char temp[]     = "Hello World.";

//
// examine /proc/maps
//
void Test1()
{
    int fd;
    fd = open("/dev/ReactorCoreServices", O_RDWR);
    if(fd < 0) 
    {
        perror("open");
        return -1;
    }

    //uint8_t*    ptr = mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_PRIVATE , fd, ALLOY_RAM_BASE );
    uint8_t*    ptr = mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_NORESERVE|MAP_PRIVATE , fd, 0 );
    printf("base = %08x physical = %08x\n", ptr, PhysicalAddressOf((uint32_t)&temp[0]));
#if 1
    for(uint32_t i=0; i<16; i++)
    {
        ptr[i] = i;
        printf("%02x ", ptr[i]);
    }
#endif
    for(uint32_t i=0; i<256; i++)
    {
        printf("%02x ", ptr[i]);
    }

    printf("\n");

    dsb(sy);
}



//
//
//
int main(int argc, char* argv[])
{
    Test1();
    exit(0);

    //Test2();
    //exit(0);

    Allocate(1024*1024);
    exit(-1);

    exec_elf(argv[1], &env);
    puts("Done");
    return 0;
}


