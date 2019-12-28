



#include "delay.h"


void delay_ms(uint32_t ms)
{
    for(uint32_t i=0; i<100000; i++);
}


void delay_us(uint32_t us)
{
    for(uint32_t i=0; i<1000; i++);
}


