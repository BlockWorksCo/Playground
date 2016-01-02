
#include <stdio.h>
#include <stdint.h>




static inline uint32_t rdtsc32(void)
{
    uint32_t r = 0;
    asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
    return r;
}





int main()
{
    while(1)
    {
        volatile uint32_t    timestamp   = rdtsc32();
        printf("%d\n", timestamp);
    }
}



