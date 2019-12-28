

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "SPIBus.h"
#include "sx1276-PhysicalInterface.h"


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

    spiBusInit();

    sx1276PhysicalInterfaceInit();

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