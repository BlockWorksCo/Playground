

#include "sx1276-PhysicalInterface.h"
#include "SPIBus.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "sx1276-RF.h" // TODO: remove the need for this


#define DIO0_IOPORT                                 GPIOA
#define DIO0_PIN                                    GPIO_Pin_1

#define DIO1_IOPORT                                 GPIOB
#define DIO1_PIN                                    GPIO_Pin_11 

#define DIO2_IOPORT                                 GPIOB
#define DIO2_PIN                                    GPIO_Pin_10

#define DIO3_IOPORT                                 GPIOB
#define DIO3_PIN                                    GPIO_Pin_1

#define DIO4_IOPORT                                 GPIOB
#define DIO4_PIN                                    GPIO_Pin_0 

#define DIO5_IOPORT                                 GPIOC
#define DIO5_PIN                                    GPIO_Pin_5

static uint8_t RFState = RF_STATE_IDLE;



/**********************************************************
**Name:     RegisterRead
**Function: SPI Read CMD
**Input:    adr -> address for read
**Output:   None
**********************************************************/
uint8_t RegisterRead(uint8_t adr)
{   
    uint8_t tmp;
    
    spiBusSelectSlave( SlaveA );
    
    spiBusWriteOneByte(adr&0x7f);
    tmp = spiBusReadOneByte();
    
    spiBusDeselectSlave( SlaveA );
    
    return(tmp);
}

/**********************************************************
**Name:     RegisterWrite
**Function: SPI Write CMD
**Input:    WrPara -> address & data
**Output:   None
**********************************************************/
void RegisterWrite(uint16_t WrPara)                
{                                                  
    spiBusSelectSlave( SlaveA );                   
    
    WrPara |= 0x8000;                                       
    spiBusWriteOneByte(WrPara>>8);//    15->0
    spiBusWriteOneByte((uint8_t)WrPara);    
    
    spiBusDeselectSlave( SlaveA );
}   


/**********************************************************
**Name:     SPIBurstRead
**Function: SPI burst read mode
**Input:    adr-----address for read
**          ptr-----data buffer point for read
**          length--how many bytes for read
**Output:   None
**********************************************************/
void SPIBurstRead(uint8_t adr, uint8_t *ptr, uint8_t length)
{
  uint8_t i;
  if(length<=1)                                            //lengt
  {
    return;
  } 
  else
  {
    spiBusSelectSlave( SlaveA );
    
    spiBusWriteOneByte(adr&0x7f);
    for(i=0;i<length;i++)
    {
        ptr[i] = spiBusWriteOneByte(0xff);
    }   
    
    spiBusDeselectSlave( SlaveA );
  } 
} 


/**********************************************************
**Name:     SPIBurstWrite
**Function: SPI burst write mode
**Input:    adr-----address for write
**          ptr-----data buffer point for write
**          length--how many bytes for write
**Output:   none
**********************************************************/
void BurstWrite(uint8_t adr, uint8_t *ptr, uint8_t length)
{ 
    spiBusSelectSlave( SlaveA );
    
    spiBusWriteOneByte(adr|0x80);
    for(uint8_t i=0;i<length;i++)
    {
        spiBusWriteOneByte(ptr[i]);
    }   
    
    spiBusDeselectSlave( SlaveA );
}   



void sx1276Reset()
{
    SX1276Reset();
}


bool sx1276IsIRQPinAsserted()
{
#if 0
    if(GPIO_ReadInputDataBit(RF_GPIO, RF_IRQ_PIN) != 0) {
        return true;
    }
    else {
        return false;
    }
#else
    return false;
#endif
}



void SX1276SetReset( uint8_t state )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if( state == RADIO_RESET_ON )
    {
        // Set RESET pin to 0
        GPIO_WriteBit( GPIOB, GPIO_Pin_12, Bit_RESET );
        // Configure RESET as output
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
        GPIO_Init( GPIOB, &GPIO_InitStructure );

        GPIOB->ODR &= ~GPIO_Pin_12;
        GPIOB->ODR &= ~GPIO_Pin_13;
    }
    else
    {
        // Configure RESET as input
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
        GPIO_Init( GPIOB, &GPIO_InitStructure );

        GPIOB->ODR |= GPIO_Pin_12;
        GPIOB->ODR |= GPIO_Pin_13;
    }
}

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;
    //NSS = 0;
    spiBusSelectSlave( SlaveA );
    spiBusWriteOneByte( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        spiBusWriteOneByte( buffer[i] );
    }
    //NSS = 1;
    spiBusDeselectSlave( SlaveA );
}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;
    //NSS = 0;
    spiBusSelectSlave( SlaveA );

    spiBusWriteOneByte( addr & 0x7F );

    for( i = 0; i < size; i++ )
    {
        buffer[i] = spiBusWriteOneByte( 0 );
    }
    spiBusDeselectSlave( SlaveA );
}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}

void SX1276SetIdleState(void)
{
 RFState=RF_STATE_IDLE;
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}

uint8_t SX1276ReadDio0( void )
{
    return GPIO_ReadInputDataBit( DIO0_IOPORT, DIO0_PIN );
}

uint8_t SX1276ReadDio1( void )
{
    return GPIO_ReadInputDataBit( DIO1_IOPORT, DIO1_PIN );
}

uint8_t SX1276ReadDio2( void )
{
    return GPIO_ReadInputDataBit( DIO2_IOPORT, DIO2_PIN );
}

uint8_t SX1276ReadDio3( void )
{
	  return GPIO_ReadInputDataBit( DIO3_IOPORT, DIO3_PIN );
}

uint8_t SX1276ReadDio4( void )
{
	  return GPIO_ReadInputDataBit( DIO4_IOPORT, DIO4_PIN );
}

uint8_t SX1276ReadDio5( void )
{
	  return GPIO_ReadInputDataBit( DIO5_IOPORT, DIO5_PIN );
}

void SX1276WriteRxTx( uint8_t txEnable )
{

}

void SX1276Reset( void )
{
    SX1276SetReset( RADIO_RESET_ON );
    delay_ms(1);
    SX1276SetReset( RADIO_RESET_OFF );
    delay_ms(10);
}



void sx1276PhysicalInterfaceInit()
{
#if 0
   GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE );

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // Configure radio DIO as inputs
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    // Configure DIO0
    GPIO_InitStructure.GPIO_Pin =  DIO0_PIN;
    GPIO_Init( DIO0_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO1
    GPIO_InitStructure.GPIO_Pin =  DIO1_PIN;
    GPIO_Init( DIO1_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO2
    GPIO_InitStructure.GPIO_Pin =  DIO2_PIN;
    GPIO_Init( DIO2_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO3 as input
    GPIO_InitStructure.GPIO_Pin =  DIO3_PIN;
    GPIO_Init( DIO3_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO5 as input
    GPIO_InitStructure.GPIO_Pin =  DIO5_PIN;
    GPIO_Init( DIO5_IOPORT, &GPIO_InitStructure );
#endif
}




