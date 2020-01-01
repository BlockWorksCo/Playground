

#include "sysclk.h"




uint32_t    sysTickCount    = 0;

void SysTick_Handler()
{
    sysTickCount++;
}

uint32_t sysclkGetTickCount()
{
    return sysTickCount;
}

