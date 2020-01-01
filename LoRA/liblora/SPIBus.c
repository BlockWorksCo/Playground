


#include "SPIBus.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"



void spiBusInit()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    //
    // Init SlaveA.
    //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    //
    // Init SlaveB.
    //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // deselect slave1
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
    GPIO_SetBits(GPIOB, GPIO_Pin_1);

    /*
     * configure pins used for SPI1
     * PA.04 = CS
     * PA.05 = SCK
     * PA.06 = MISO (Master In/Slave Out)
     * PA.07 = MOSI (Master Out/Slave In)
     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*
     * The MISO, MOSI and CLK pins are now connected to their AF
     * so that the SPI1 can take over control of the pins
     */
    GPIO_PinLockConfig(GPIOA, GPIO_Pin_5);
    GPIO_PinLockConfig(GPIOA, GPIO_Pin_6);
    GPIO_PinLockConfig(GPIOA, GPIO_Pin_7);

    //
    // Init the SPI1 peripheral.
    //
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
    SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}

void spiBusSelectSlave( SPISlaveID id )
{
    if( id == SlaveA )
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    }
}

void spiBusDeselectSlave( SPISlaveID id )
{
    if( id == SlaveA )
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_0);
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_1);
    }
}

uint8_t spiBusWriteOneByte( uint8_t byte )
{
    /* Loop while DR register in not emplty */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    
    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI1, byte);
    
    /* Wait to receive a byte */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);
}

uint8_t spiBusReadOneByte()
{
    return spiBusWriteOneByte(0xff);
}



