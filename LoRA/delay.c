



#include "delay.h"


void delay_ms(uint32_t ms)
{
    delay_us(1000*ms);
}


void delay_us(uint32_t us)
{
    for(uint32_t i=0; i<4*us; i++);
}


