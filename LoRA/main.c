

#include "BoardSupport.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "sx1276-PhysicalInterface.h"
#include "sx1276-LoRa.h"
#include "EventQueue.h"
#include "TimedEvents.h"


//
//
//
void tick()
{
    GPIOC->ODR ^= GPIO_Pin_13; // Invert C13
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
    //
    // Receive any packets on SlaveA.
    //
    if ( loraCheckAsyncReceiveCompletion(SlaveA) == true )
    {
        static uint8_t receiveBuffer[128]  = {0};
        uint8_t length  = loraReceivePacket( SlaveA, &receiveBuffer[0], sizeof(receiveBuffer) );
        if(length > 0) 
        {
            Call(FlashLED_A);
        }
    }

    if(loraCheckAsyncTransmitForCompletion(SlaveA) == true)
    {
        uint8_t     packet[8]  = {0,2,3,4,5,6};
        loraTransmitPacket( SlaveA,  &packet[0], sizeof(packet) );
    }
}


//
// Transmit from SlaveB
//
void SlaveBTransmit()
{
    //
    // Receive any packets on SlaveB.
    //
    if ( loraCheckAsyncReceiveCompletion(SlaveB) == true )
    {
        static uint8_t receiveBuffer[128]  = {0};
        uint8_t length  = loraReceivePacket( SlaveB, &receiveBuffer[0], sizeof(receiveBuffer) );
        if(length > 0) 
        {
            Call(FlashLED_B);
        }
    }

    if(loraCheckAsyncTransmitForCompletion(SlaveB) == true)
    {
        uint8_t     packet[8]  = {0,2,3,4,5,6};
        loraTransmitPacket( SlaveB,  &packet[0], sizeof(packet) );
    }
}

void StartSlaveB()
{
    CallEvery_ms( SlaveBTransmit, 5000 );
}



//
// Check whether the LoRa modules have completed transmission
// and if so, move them back into receive mode.
//
void PollForTransmitCompletion()
{
    loraCheckAsyncTransmitForCompletion(SlaveA);
    loraCheckAsyncTransmitForCompletion(SlaveB);
}


//
//
//
int main(void)
{
    BoardSupportInitialise();

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
    // Startup the radio(s).
    //
    sx1276PhysicalInterfaceInit( SlaveA );
    loraContinuousReceiveMode( SlaveA );

    sx1276PhysicalInterfaceInit( SlaveB );
    loraContinuousReceiveMode( SlaveB );

    //
    // Startup flash of both LEDs and continuous toggle of
    // on-board LED.
    //
    Call( FlashAllLEDs );
    CallEvery_ms( tick, 200 );

    //
    //
    //
    CallEvery_ms( SlaveATransmit, 5000 );
    CallAfter_ms( StartSlaveB, 2000 );
    CallEvery_ms( PollForTransmitCompletion, 100 );

    //
    // Forever...
    // 
    while(true) 
    {
        CheckTimedEventHandlers();
        DispatchHandlers();
    }
}




