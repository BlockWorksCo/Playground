//
//
//



#include <stdint.h>
#include <stdbool.h>
#include "Configuration.h"



TransferChannelType                             transferController;


TimingType                                  timing;



//
// Raw pin definitions.
//


Pin0Type                    pin0;
Pin1Type                    pin1;
Pin2Type                    pin2;
Pin3Type                    pin3;
Pin4Type                    pin4;
Pin5Type                    pin5;
Pin6Type                    pin6;
Pin7Type                    pin7;




//
//
//
void RunSchedule()
{
    pin0Controller.Process();       // 60ns
    pin1Controller.Process();       // 60ns
    pin2Controller.Process();       // 130ns
pin6.Set();
    pin3Controller.Process();       // 160ns
pin6.Clear();
    pin4Controller.Process();       // 130ns
    pin5Controller.Process();       // 60ns
    //pin6Controller.Process();     // 130ns
    pin7Controller.Process();       // 130ns
}




extern unsigned long __preinit_array_start;
extern unsigned long __preinit_array_end;
extern unsigned long __init_array_start;
extern unsigned long __init_array_end;
extern unsigned long _ctor_start;
extern unsigned long _ctor_end;
static void call_constructors(unsigned long *start, unsigned long *end) __attribute__((noinline));
static void call_constructors(unsigned long *start, unsigned long *end)
{
  unsigned long *i;
  void (*funcptr)();
  for ( i = start; i < end; i++)
  {
    funcptr=(void (*)())(*i);
    funcptr();
  }
}




extern "C" void __cxa_pure_virtual() 
{ 
    while (1); 
}



extern "C" void abort(void)
{
    while(true);
}







enum sysclk_freq {
    SYSCLK_42_MHZ=0,
    SYSCLK_84_MHZ,
    SYSCLK_168_MHZ,
    SYSCLK_200_MHZ,
    SYSCLK_240_MHZ,
};
 
void rcc_set_frequency(enum sysclk_freq freq)
{
    int freqs[]   = {42, 84, 168, 200, 240};
 
    /* USB freqs: 42MHz, 42Mhz, 48MHz, 50MHz, 48MHz */
    int pll_div[] = {2, 4, 7, 10, 10}; 
 
    /* PLL_VCO = (HSE_VALUE / PLL_M) * PLL_N */
    /* SYSCLK = PLL_VCO / PLL_P */
    /* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
    uint32_t PLL_P = 2;
    uint32_t PLL_N = freqs[freq] * 2;
    uint32_t PLL_M = (HSE_VALUE/1000000);
    uint32_t PLL_Q = pll_div[freq];
 
    RCC_DeInit();
 
    /* Enable HSE osscilator */
    RCC_HSEConfig(RCC_HSE_ON);
 
    if (RCC_WaitForHSEStartUp() == ERROR) {
        return;
    }
 
    /* Configure PLL clock M, N, P, and Q dividers */
    RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q);
 
    /* Enable PLL clock */
    RCC_PLLCmd(ENABLE);
 
    /* Wait until PLL clock is stable */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);
 
    /* Set PLL_CLK as system clock source SYSCLK */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
    /* Set AHB clock divider */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
 
    /* Set APBx clock dividers */
    switch (freq) {
        /* Max freq APB1: 42MHz APB2: 84MHz */
        case SYSCLK_42_MHZ:
            RCC_PCLK1Config(RCC_HCLK_Div1); /* 42MHz */
            RCC_PCLK2Config(RCC_HCLK_Div1); /* 42MHz */
            break;
        case SYSCLK_84_MHZ:
            RCC_PCLK1Config(RCC_HCLK_Div2); /* 42MHz */
            RCC_PCLK2Config(RCC_HCLK_Div1); /* 84MHz */
            break;
        case SYSCLK_168_MHZ:
            RCC_PCLK1Config(RCC_HCLK_Div4); /* 42MHz */
            RCC_PCLK2Config(RCC_HCLK_Div2); /* 84MHz */
            break;
        case SYSCLK_200_MHZ:
            RCC_PCLK1Config(RCC_HCLK_Div4); /* 50MHz */
            RCC_PCLK2Config(RCC_HCLK_Div2); /* 100MHz */
            break;
        case SYSCLK_240_MHZ:
            RCC_PCLK1Config(RCC_HCLK_Div4); /* 60MHz */
            RCC_PCLK2Config(RCC_HCLK_Div2); /* 120MHz */
            break;
    }
 
    /* Update SystemCoreClock variable */
    SystemCoreClockUpdate();
}






//
//
//
int main()
{
    SystemInit();

    rcc_set_frequency(SYSCLK_168_MHZ);

    //
    //Call C++ global constructors
    //
    call_constructors(&__preinit_array_start, &__preinit_array_end);
    call_constructors(&__init_array_start, &__init_array_end);

    //
    // Forever
    //
    while(true)
    {
        //
        // Calculate the new outputs.
        // Note: The PinControllers are idempotent, so can be applied repeatedly.
        //
        RunSchedule();            

        //
        // Data transfer.
        //
        transferController.Go();
    }

    return 0;
}



extern "C" void WWDG_IRQHandler(void)
{
    while(true);
}