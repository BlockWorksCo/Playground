

#include "sx1276-PhysicalInterface.h"
#include "SPIBus.h"


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
#if 0
    GPIO_ResetBits(GPIOA, RF_RST);//REST_Low XL1278-D01;
    GPIO_ResetBits(GPIOB, RF_RST_1);//REST_Low  XL1278-SMT;

    for(i=100;i!=0;i--)                                      //Del
    {
        delay_us(10); 
    }
  
    GPIO_SetBits(GPIOA, RF_RST);//REST_High XL1278-D01;
    GPIO_SetBits(GPIOB, RF_RST_1);//REST_High XL1278-SMT;

    for(i=250;i!=0;i--)                                      //Del
    {
        delay_us(10);  
    }
#endif
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


