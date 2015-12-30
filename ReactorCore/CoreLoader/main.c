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

int main(int argc, char* argv[])
{
    exec_elf(argv[1], &env);
    puts("Done");
    return 0;
}

//
//
//
unsigned long get_page_frame_number_of_address(void *addr) 
{
   // Open the pagemap file for the current process
   FILE *pagemap = fopen("/proc/self/pagemap", "rb");

   // Seek to the page that the buffer is on
   unsigned long offset = (unsigned long)addr / getpagesize() * PAGEMAP_LENGTH;
   if(fseek(pagemap, (unsigned long)offset, SEEK_SET) != 0) 
   {
      fprintf(stderr, "Failed to seek pagemap to proper location\n");
      exit(1);
   }

   // The page frame number is in bits 0-54 so read the first 7 bytes and clear the 55th bit
   unsigned long page_frame_number = 0;
   fread(&page_frame_number, 1, PAGEMAP_LENGTH-1, pagemap);

   page_frame_number &= 0x7FFFFFFFFFFFFF;

   fclose(pagemap);

   return page_frame_number;
}


//
//
//
void *do_alloc(size_t size, size_t align, ELFSecPerm_t perm)
{
    (void) perm;
    
    //
    // Allocate the block, aligned on page size..
    //
    void*   block   = memalign(32768, size);

    //
    // Set the attributes to allow execution.
    //
    int     r       = mprotect(block, size, PROT_EXEC|PROT_READ|PROT_WRITE);
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
    uint32_t    physicalAddress     = (pageFrameNumber << PAGE_SHIFT) + 0;

    return block;
}

void arch_jumpTo(entry_t entry)
{
    static uint8_t  temp[100]   = {0};
    register uint32_t        r11         = (uint32_t)&temp[50];
    __asm__ volatile("MOV r11, %0\n\t" : : "r"(r11));
    entry();
#if 0
    void *stack = do_alloc(APP_STACK_SIZE, 8, ELF_SEC_READ | ELF_SEC_WRITE);
    if (stack) {
        register uint32_t saved = 0;
        void *tos = stack + APP_STACK_SIZE;

        /* s->saved */
        __asm__ volatile("MOV %0, sp\n\t" : : "r"(saved));
        /* tos->MSP */
        __asm__ volatile("MOV sp, %0\n\t" : : "r"(tos));
        /* push saved */
        __asm__ volatile("PUSH {%0}\n\t" : : "r"(saved));

        entry();

        /* pop saved */
        __asm__ volatile("POP {%0}\n\t" : : "r"(saved));
        /* saved->sp */
        __asm__ volatile("MOV sp, %0\n\t" : : "r"(saved));

            free(stack);
        } else
            perror("Stack alloc");
#endif      
}
