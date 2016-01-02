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

    printf("physical address = %08x virtual address = %08x\n", *physicalAddress, (uint32_t)block );

    return block;
}

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
//
//
void arch_jumpTo(entry_t entry)
{
    printf("EntryPoint @ %08x \n", (uint32_t)entry );

    //
    // Trigger the core to execute this code.
    //
    //0x400000CC+(cpuNumber<<4)
    int     fd  = open("/dev/mem", O_RDWR|O_SYNC);
    if(fd != -1)
    {
        uint32_t    cpuNumber   = 3;
        uint32_t    offset      = 0xCC+(cpuNumber<<4);

        uint32_t*   map = (uint32_t*)mmap( NULL,
                            4096,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd,             // File descriptor to physical memory virtual file '/dev/mem'
                            0x40000000      // Address in physical map that we want this memory block to expose
                            );

        if (map == MAP_FAILED) 
        {
            ERR("mmap failed.");
        }

        printf("Writing %08x to address %08x\n",(uint32_t)entry, offset);
        map[0xfc/4] = (uint32_t)entry;
        msync( map , 4096 , MS_SYNC);

        sleep(1);
        for(uint32_t i=0; i<128; i++)
        {
            printf("%04x) %08x\n", i*4, map[i]);
        }



#if 0
        uint32_t pos = lseek(fd, offset, SEEK_SET);
        write(fd, &entry, sizeof(uint32_t));
        pos = lseek(fd, offset, SEEK_SET);
        read(fd, &entry, sizeof(uint32_t));
        printf("read %08x from address %08x\n",(uint32_t)entry, offset);
#endif        
        close(fd);
    }
    else
    {
        ERR("Cant open /dev/mem.");
    }

    //
    // Wait for completion.
    //
    while(true)
    {
        sleep(1);
    }
}





//
//
//
int main(int argc, char* argv[])
{
    exec_elf(argv[1], &env);
    puts("Done");
    return 0;
}


