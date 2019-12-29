

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "SPIBus.h"
#include "sx1276-PhysicalInterface.h"
#include "sx1276-LoRa.h"
#include "delay.h"

void ReadTemperature();
uint8_t receiveBuffer[128]  = {0};





uint8_t loraReceivePacket( uint8_t* buf )
{
    uint8_t length = 0;

    if ( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_10 ) != 0 )
    {
        length = RFM96_LoRaRxPacket( &receiveBuffer[0] );

        RFM96_LoRaEntryRx();
    }

    return length;
}


uint8_t loraTransmitPacket( uint8_t* buf, uint8_t size )
{
    int ret = 0;

    ret = RFM96_LoRaEntryTx(size);
    ret = RFM96_LoRaTxPacket(buf,size);

    delay_ms( 10 );
    RFM96_LoRaEntryRx();

    return ret;
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

  	GPIOB->ODR ^= GPIO_Pin_15; // Invert C13
    delay_ms(1000);
  	GPIOB->ODR ^= GPIO_Pin_15; // Invert C13

    sx1276PhysicalInterfaceInit();

    RFM96_LoRaEntryRx();

  while (1) {

    	/* Toggle LED which connected to PC13*/
        if(SX1276ReadDio0() == true) {
        	GPIOB->ODR ^= GPIO_Pin_15; // Invert C13
        }
    	GPIOB->ODR ^= GPIO_Pin_14; // Invert C13
    	GPIOC->ODR ^= GPIO_Pin_13; // Invert C13

    	/* delay */
        delay_ms(100);

    	/* Toggle LED which connected to PC13*/
    	GPIOB->ODR ^= GPIO_Pin_14; // Invert C13
    	GPIOC->ODR ^= GPIO_Pin_13; // Invert C13

        ReadTemperature();

    	/* delay */
        delay_ms(100);

        //
        //
        //
        loraReceivePacket( &receiveBuffer[0] );
  }
}
