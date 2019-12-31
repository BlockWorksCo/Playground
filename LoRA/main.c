

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "SPIBus.h"
#include "sx1276-PhysicalInterface.h"
#include "sx1276-LoRa.h"
#include "delay.h"
#include <string.h>

void ReadTemperature();
uint8_t receiveBuffer[128]  = {0};





uint8_t loraReceivePacket( SPISlaveID id, uint8_t* buf )
{
    uint8_t length = 0;

    if ( loraCheckAsyncReceiveCompletion(id) == true )
    {
        memset( &receiveBuffer[0], 0xff, sizeof(receiveBuffer) );
        length = RFM96_LoRaRxPacket( id, &receiveBuffer[0] );

        RFM96_LoRaEntryRx( id );
    }

    return length;
}


void loraTransmitPacket( SPISlaveID id, uint8_t* buf, uint8_t size )
{
    RFM96_LoRaEntryTx(id, size);
    loraTransmitPacket_Async(id, buf,size);
}




int main(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Initialize LED which connected to PC13 */
  // Enable PORTC Clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Initialize LED which connected to PC13 */
  // Enable PORTC Clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Set C13 to Low level ("0")
  GPIO_ResetBits(GPIOB, GPIO_Pin_14); // Set C13 to Low level ("0")
  GPIO_ResetBits(GPIOB, GPIO_Pin_15); // Set C13 to Low level ("0")

  /* Initialize Button input PB10 PB11*/
  // Enable PORTB Clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  /* Configure the GPIO_BUTTON pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    spiBusInit();

    GPIOB->ODR ^= GPIO_Pin_14; // Invert C13
    GPIOB->ODR ^= GPIO_Pin_15; // Invert C13
    delay_ms(1000);
    GPIOB->ODR ^= GPIO_Pin_14; // Invert C13
    GPIOB->ODR ^= GPIO_Pin_15; // Invert C13

    sx1276PhysicalInterfaceInit( SlaveA );
    sx1276PhysicalInterfaceInit( SlaveB );

    RFM96_LoRaEntryRx( SlaveA );
    RFM96_LoRaEntryRx( SlaveB );

  while (1) {

    	/* Toggle LED which connected to PC13*/
    	GPIOC->ODR ^= GPIO_Pin_13; // Invert C13

    	/* delay */
        delay_ms(100);

    	/* Toggle LED which connected to PC13*/
    	GPIOC->ODR ^= GPIO_Pin_13; // Invert C13

        ReadTemperature();

    	/* delay */
        delay_ms(100);

        //
        // Receive any packets on SlaveA.
        //
        {
            uint8_t length  = loraReceivePacket( SlaveA, &receiveBuffer[0] );
            if(length > 0) {
                GPIOB->ODR |= GPIO_Pin_14; // Invert C13
                delay_ms(100);
                GPIOB->ODR &= ~GPIO_Pin_14; // Invert C13
            }
        }

        //
        // Receive any packets on SlaveB.
        //
        {
            uint8_t length  = loraReceivePacket( SlaveB, &receiveBuffer[0] );
            if(length > 0) {
                GPIOB->ODR |= GPIO_Pin_15; // Invert C13
                delay_ms(100);
                GPIOB->ODR &= ~GPIO_Pin_15; // Invert C13
            }
        }

        //
        // Transmit from SlaveA
        //
        if(loraCheckAsyncTransmitForCompletion(SlaveA) == true)
        {
            static uint32_t count    = 0;
            count++;
            if(count > 6) {
                static uint8_t  i   = 0;
                count   = 0;
                uint8_t     packet[8]  = {0,2,3,4,5,6};
                packet[0]   = i;
                i++;
                loraTransmitPacket( SlaveA,  &packet[0], sizeof(packet) );
            }
        }

        //
        // Transmit from SlaveB
        //
        if(loraCheckAsyncTransmitForCompletion(SlaveB) == true)
        {
            static uint32_t count    = 0;
            count++;
            if(count > 20) {
                static uint8_t  i   = 0;
                count   = 0;
                uint8_t     packet[8]  = {0,1,2,3,4,5};
                packet[0]   = i;
                i++;
                loraTransmitPacket( SlaveB,  &packet[0], sizeof(packet) );
            }
        }
  }
}
