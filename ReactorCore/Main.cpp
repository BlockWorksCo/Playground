//
//
//



#include <stdint.h>
#include <stdbool.h>
#include "Configuration.h"



TransferChannelType                             transferChannel;


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
    pin0Controller.Process();
    pin1Controller.Process();
    pin2Controller.Process();
    pin3Controller.Process();
    pin4Controller.Process();
    pin5Controller.Process();
    pin6Controller.Process();
    pin7Controller.Process();
}


//
// Rx and Tx data from/to the SPI port.
//
void TransferUserData()
{

}


#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include "stm32f4xx.h"
#include "core_cm4.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

//
//
//
int main()
{
    //
    //  Initialise the peripheral clock.
    //
    RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
    //
    //  Initilaise the GPIO port.
    //
    GPIOD->MODER |= GPIO_Mode_OUT;
    GPIOD->OSPEEDR |= GPIO_Speed_25MHz;
    GPIOD->OTYPER |= GPIO_OType_PP;
    GPIOD->PUPDR |= GPIO_PuPd_NOPULL;

    ((CoreDebug_Type*)CoreDebug_BASE)->DEMCR |= 0x01000000;
    ((DWT_Type*)DWT_BASE)->CYCCNT      = 0;            // reset the counter
    ((DWT_Type*)DWT_BASE)->CTRL        |= 0x00000001;  // enable the counter


    while(true)
    {
        uint32_t    previousTimestamp       = 0;
        volatile uint32_t    nanosecondTimestamp     = timing.GetNanosecondTick();

        if( (nanosecondTimestamp%168) < 10)
        {
            GPIOD->ODR  = ~GPIOD->ODR;
        }
        //pin0.Set();
        //pin0.Clear();
    }

    //
    // Forever
    //
    while(true)
    {
        uint32_t    previousTimestamp       = 0;
        uint32_t    nanosecondTimestamp     = timing.GetNanosecondTick();

        //
        // Calculate the new outputs.
        //
        if(nanosecondTimestamp !=  previousTimestamp)
        {
            RunSchedule();            
        }

        //
        // Data transfer.
        //
        transferChannel.Go();

        //
        //
        //
        previousTimestamp   = nanosecondTimestamp;
    }

    return 0;
}



extern "C" void WWDG_IRQHandler(void)
{
    while(true);
}