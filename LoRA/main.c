#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"


void InitSPI()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    //
    // Init SlaveSelect1.
    //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //
    // Init SlaveSelect2.
    //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //
    // Init the SPI1 peripheral.
    //
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
    SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}

int main(void)
{
  int i;
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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //GPIO_SetBits(GPIOC, GPIO_Pin_13); // Set C13 to High level ("1")
  GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Set C13 to Low level ("0")
  GPIO_ResetBits(GPIOB, GPIO_Pin_10); // Set C13 to Low level ("0")

#if 0
  /* Initialize Button input PB0 */
  // Enable PORTB Clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  /* Configure the GPIO_BUTTON pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif

    InitSPI();

  while (1) {

    	/* Toggle LED which connected to PC13*/
    	GPIOB->ODR ^= GPIO_Pin_10; // Invert C13
    	GPIOC->ODR ^= GPIO_Pin_13; // Invert C13

    	/* delay */
    	for(i=0;i<0x100000;i++);

    	/* Toggle LED which connected to PC13*/
    	GPIOB->ODR ^= GPIO_Pin_10; // Invert C13
    	GPIOC->ODR ^= GPIO_Pin_13; // Invert C13

    	/* delay */
    	for(i=0;i<0x100000;i++);

#if 0
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) != 0) {
    	/* Toggle LED which connected to PC13*/
    	GPIOC->ODR ^= GPIO_Pin_13; // Invert C13

    	/* delay */
    	for(i=0;i<0x100000;i++);

    	/* Toggle LED which connected to PC13*/
    	GPIOC->ODR ^= GPIO_Pin_13;

    	/* delay */
    	for(i=0;i<0x100000;i++);
    }
    else {
    	GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
#endif

  }
}
