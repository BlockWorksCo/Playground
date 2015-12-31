


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/mman.h>
#include <stdint.h>
#include "loader.h"



#define PAGEMAP_LENGTH  8
#define PAGE_SHIFT      12



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


//
//
//
void arch_jumpTo(entry_t entry)
{
    printf("EntryPoint @ %08x \n", (uint32_t)entry );
    entry();
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


