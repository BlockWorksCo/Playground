

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "sx1276-PhysicalInterface.h"
#include "sx1276-LoRa.h"
#include "delay.h"
#include <string.h>
#include "sysclk.h"




int main(void)
{
    //
    // Initialise GPIOs.
    //
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Set C13 to Low level ("0")
    GPIO_ResetBits(GPIOB, GPIO_Pin_14); // Set C13 to Low level ("0")
    GPIO_ResetBits(GPIOB, GPIO_Pin_15); // Set C13 to Low level ("0")

    //
    // Initialise SysTick.
    //
    //SysTick_Config(SystemCoreClock/100);
    SysTick_Config(50000);


    //
    // Startup flash of both LEDs.
    //
    GPIOB->ODR ^= GPIO_Pin_14; // Invert C13
    GPIOB->ODR ^= GPIO_Pin_15; // Invert C13
    delay_ms(1000);
    GPIOB->ODR ^= GPIO_Pin_14; // Invert C13
    GPIOB->ODR ^= GPIO_Pin_15; // Invert C13

    //
    // Startup the radio(s).
    //
    sx1276PhysicalInterfaceInit( SlaveA );
    loraContinuousReceiveMode( SlaveA );

    sx1276PhysicalInterfaceInit( SlaveB );
    loraContinuousReceiveMode( SlaveB );

    //
    // Forever...
    // 
    while(true) 
    {
        static uint8_t receiveBuffer[128]  = {0};

        /* Toggle LED which connected to PC13*/
        GPIOC->ODR ^= GPIO_Pin_13; // Invert C13

        //
        // Receive any packets on SlaveA.
        //
        if ( loraCheckAsyncReceiveCompletion(SlaveA) == true )
        {
            uint8_t length  = loraReceivePacket( SlaveA, &receiveBuffer[0], sizeof(receiveBuffer) );
            if(length > 0) {
                GPIOB->ODR |= GPIO_Pin_14; // Invert C13
                delay_ms(100);
                GPIOB->ODR &= ~GPIO_Pin_14; // Invert C13
            }
        }

        //
        // Transmit from SlaveA
        //
        if(loraCheckAsyncTransmitForCompletion(SlaveA) == true)
        {
            static uint32_t count    = 0;
            if((sysclkGetTickCount()-count) > 6300) {
                count   = sysclkGetTickCount();
                uint8_t     packet[8]  = {0,2,3,4,5,6};
                loraTransmitPacket( SlaveA,  &packet[0], sizeof(packet) );
            }
        }



        //
        // Receive any packets on SlaveB.
        //
        if ( loraCheckAsyncReceiveCompletion(SlaveB) == true )
        {
            uint8_t length  = loraReceivePacket( SlaveB, &receiveBuffer[0], sizeof(receiveBuffer) );
            if(length > 0) {
                GPIOB->ODR |= GPIO_Pin_15; // Invert C13
                delay_ms(100);
                GPIOB->ODR &= ~GPIO_Pin_15; // Invert C13
            }
        }

        //
        // Transmit from SlaveB
        //
        if(loraCheckAsyncTransmitForCompletion(SlaveB) == true)
        {
            static uint32_t count    = 0;
            if((sysclkGetTickCount()-count) > 5700) {
                count   = sysclkGetTickCount();
                uint8_t     packet[8]  = {0,1,2,3,4,5};
                loraTransmitPacket( SlaveB,  &packet[0], sizeof(packet) );
            }
        }


    }
}




