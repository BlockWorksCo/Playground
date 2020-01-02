



#include "delay.h"
#include "BoardSupport.h"


void delay_ms(uint32_t ms)
{
    uint32_t    start   = GetTickCount();
    while(GetTickCount() < (start+ms) );
}


void delay_us(uint32_t us)
{
    // minimum delay of 1ms.
    delay_ms(1);
}


