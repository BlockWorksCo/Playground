

#include "BoardSupport.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "EventQueue.h"
#include "TimedEvents.h"
#include "Radio.h"
#include "ErrorHandling.h"


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
// Transmit from SlaveA
//
void SlaveATransmit()
{
    uint8_t     packet[8]  = {0,2,3,4,5,6};
    radioAsyncTransmitPacket( RadioA,  &packet[0], sizeof(packet) );
    Call(FlashLED_B);
}


//
// Transmit from SlaveB
//
void SlaveBTransmit()
{
    uint8_t     packet[8]  = {0,2,3,4,5,6};
    radioAsyncTransmitPacket( RadioB,  &packet[0], sizeof(packet) );
}

void StartSlaveB()
{
    CallEvery_ms( SlaveBTransmit, 3000 );
}



//
// Check whether the LoRa modules have completed transmission
// and if so, move them back into receive mode.
//
void PollForTransmitCompletion()
{
    radioAsyncTransmitCompleted(RadioA);
    radioAsyncTransmitCompleted(RadioB);
}


//
//
//
void PollForPacketReception()
{
#if 0
    //
    // Receive any packets on RadioB.
    //
    if ( radioAsyncReceiveCompleted(RadioB) == true )
    {
        static uint8_t receiveBuffer[128]  = {0};
        uint8_t length  = radioAsyncReceivePacket( RadioB, &receiveBuffer[0], sizeof(receiveBuffer) );
        if(length > 0) 
        {
            Call(FlashLED_B);
        }
        else
        {
            PANIC("ouch!");
        }
    }
#endif
    //
    // Receive any packets on RadioA.
    //
    if ( radioAsyncReceiveCompleted(RadioA) == true )
    {
        static uint8_t receiveBuffer[128]  = {0};
        uint8_t length  = radioAsyncReceivePacket( RadioA, &receiveBuffer[0], sizeof(receiveBuffer) );
        if(length > 0) 
        {
            Call(FlashLED_A);
        }
        else
        {
            PANIC("ouch!");
        }
    }
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
    radioInitialise( RadioA );
    //radioInitialise( RadioB );

    //
    // Startup flash of both LEDs and continuous toggle of
    // on-board LED.
    //
    Call( FlashAllLEDs );
    CallEvery_ms( tick, 200 );

    //
    //
    //
    //CallAfter_ms( StartSlaveB, 1500 );
    CallEvery_ms( SlaveATransmit, 3000 );
    CallEvery_ms( PollForTransmitCompletion, 100 );
    CallEvery_ms( PollForPacketReception, 50 );

    //
    // Forever...
    // 
    while(true) 
    {
        CheckTimedEventHandlers();
        DispatchHandlers();
    }
}




