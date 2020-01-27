

#include "BoardSupport.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "EventQueue.h"
#include "TimedEvents.h"
#include "ErrorHandling.h"
#include "Ethernet.h"


//
//
//
void tick()
{
    GPIOC->ODR ^= GPIO_Pin_13;
}


//
//
//
void TurnOffAllLEDs()
{
    GPIOB->ODR &= ~GPIO_Pin_14;
    GPIOB->ODR &= ~GPIO_Pin_15;
}

void FlashAllLEDs()
{
    GPIOB->ODR |= GPIO_Pin_14;
    GPIOB->ODR |= GPIO_Pin_15;

    CallAfter_ms( TurnOffAllLEDs, 1000 );
}



//
//
//
void TurnOffLED_A()
{
    GPIOB->ODR &= ~GPIO_Pin_14;
}

void FlashLED_A()
{
    GPIOB->ODR |= GPIO_Pin_14;

    CallAfter_ms( TurnOffLED_A, 100 );
}




//
//
//
void TurnOffLED_B()
{
    GPIOB->ODR &= ~GPIO_Pin_15;
}

void FlashLED_B()
{
    GPIOB->ODR |= GPIO_Pin_15;

    CallAfter_ms( TurnOffLED_B, 100 );
}





//
//
//
void InitialiseLEDs()
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
}


//
//
//
int main(void)
{
    BoardSupportInitialise();

    InitialiseLEDs();

    //
    // Startup the radio(s).
    //
    //radioInitialise( RadioA );
    //radioInitialise( RadioB );

    //
    // Startup flash of both LEDs and continuous toggle of
    // on-board LED.
    //
    Call( FlashAllLEDs );
    CallEvery_ms( tick, 200 );

    //
    ethernetInit( SlaveA );

    //
    //
    //
    //CallAfter_ms( StartSlaveB, 1500 );
    //CallEvery_ms( SlaveATransmit, 3000 );
    //CallEvery_ms( PollForTransmitCompletion, 100 );
    //CallEvery_ms( PollForPacketReception, 50 );

    //
    // Forever...
    // 
    while(true) 
    {
        CheckTimedEventHandlers();
        DispatchHandlers();
    }
}




