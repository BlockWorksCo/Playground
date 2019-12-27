/******************** (C) COPYRIGHT tongxinmao.com ***************************
* 文件名		: sx1276-LoRa.C
* 作者   		: tcm123@126.com
* 创建日期  	: 2016.3.8
* 描述			: SX1276/78驱动文件 
* 版本			: V1.0
* 历史记录  	: 暂无
********************************************************************************/

#include "sx1276-LoRa.h"
#include "sx1276-PhysicalInterface.h"
#include "delay.h"
#include "SPIBus.h"
#include <stdio.h>




#define gb_SF  3
#define gb_BW  6
#define CR    0x04 
#define CRC_EN   0x00  //CRC Enable

 
/**********************************************************
**Parameter table define
**********************************************************/
 const uint16_t RFM96FreqTbl[3] = {0x066C, 0x0780, 0x0800}; //434MHz  32M晶振0x066C, 0x0780, 0x0800 	   30M:0x0673, 0x07BB, 0x08BB   {0x85,0x89,0xd9},//26M-TCXO 434M

 const uint16_t RFM96PowerTbl[4] =
{ 
  0x09FF,                   //20dbm  
  0x09FC,                   //17dbm
  0x09F9,                   //14dbm
  0x09F6,                   //11dbm 
};

 /*
 const uint8_t RFM96SpreadFactorTbl[7] =
{
  6,7,8,9,10,11,12
   
}; */

const uint8_t RFM96SpreadFactorTbl[6] =
{
  7,8,9,10,11,12
};//由于6扩频因子档位只能是固定数据包长所以去掉6扩频因子档位

 const uint8_t RFM96LoRaBwTbl[10] =
{// 0        1     2       3      4       5         6      7       8      9
//7.8KHz,10.4KHz,15.6KHz,20.8KHz,31.2KHz,41.7KHz,62.5KHz,125KHz,250KHz,500KHz
  0,1,2,3,4,5,6,7,8,9
};

 const uint8_t  RFM96Data[] = {"1234567890ABCDEFGHIJK"};







/**********************************************************
**Name:     RFM96_Standby
**Function: Entry standby mode
**Input:    None
**Output:   None
**********************************************************/
void RFM96_Standby(void)
{
  RegisterWrite(LR_RegOpMode+0x01+0x08);                              //Standby
}

/**********************************************************
**Name:     RFM96_Sleep
**Function: Entry sleep mode
**Input:    None
**Output:   None
**********************************************************/
void RFM96_Sleep(void)
{
  RegisterWrite(LR_RegOpMode+0x00+0x08);                              //Sleep
}

/*********************************************************/
//LoRa mode
/*********************************************************/
/**********************************************************
**Name:     RFM96_EntryLoRa
**Function: Set RFM69 entry LoRa(LongRange) mode
**Input:    None
**Output:   None
**********************************************************/
void RFM96_EntryLoRa(void)
{
  RegisterWrite(LR_RegOpMode+0x80+0x08);
}

/**********************************************************
**Name:     RFM96_LoRaClearIrq
**Function: Clear all irq
**Input:    None
**Output:   None
**********************************************************/
void RFM96_LoRaClearIrq(void)
{
  RegisterWrite(LR_RegIrqFlags+0xFF);
}

/**********************************************************
**Name:     RFM96_Config
**Function: RFM96 base config
**Input:    mode
**Output:   None
**********************************************************/
void RFM96_Config(uint8_t mode)
{
  uint8_t i; 
    
    sx1276Reset();
    
  RFM96_Sleep();                                           //Change modem mode Must in Sleep mode 
  for(i=250;i!=0;i--)                                      //Delay
  {
    delay_us(1);  
  }


	  
    RFM96_EntryLoRa();  
   // RegisterWrite(0x5904);   //?? Change digital regulator form 1.6V to 1.47V: see errata note


     {
		//volatile uint8_t	Sx1276VerNO = RegisterRead((uint8_t)(REG_LR_VERSION>>8)); //获取SX1276 版本号 是0X11(V1A版本 工程版） 或者是 0X12（V1B 正式版）
//		 printf("sx1278(V12) v:%02X",Sx1276VerNO);
	//	 RegisterWrite(RFM96FreqTbl[0]); 
	//	 Sx1276VerNO=RegisterRead((uint8_t)(RFM96FreqTbl[0]>>8));
	 //   printf("RFM96FreqTbl[0](0X06_6C) R:0x%02X",Sx1276VerNO);

       	}
    for(i=0;i<3;i++)                                       //setting frequency parameter
    {
      RegisterWrite(RFM96FreqTbl[i]);  
    }

    //setting base parameter 
    RegisterWrite(RFM96PowerTbl[0]);             //Setting output power parameter      
    RegisterWrite(LR_RegOcp+0x0B);                              //RegOcp,Close Ocp
    RegisterWrite(LR_RegLna+0x23);                              //RegLNA,High & LNA Enable
    
    if(RFM96SpreadFactorTbl[gb_SF]==6)           //SFactor=6
    {
      uint8_t tmp;											
      RegisterWrite(LR_RegModemConfig1+(RFM96LoRaBwTbl[gb_BW]<<4)+(CR<<1)+0x01);//带宽设置，包结构，Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
      RegisterWrite(LR_RegModemConfig2+(RFM96SpreadFactorTbl[gb_SF]<<4)+(CRC_EN<<2)+0x03);      
      tmp = RegisterRead(0x31);
      tmp &= 0xF8;
      tmp |= 0x05;
      RegisterWrite(0x3100+tmp);
      RegisterWrite(0x3700+0x0C);
    } 
    else
    {
      RegisterWrite(LR_RegModemConfig1+(RFM96LoRaBwTbl[gb_BW]<<4)+(CR<<1)+0x00);//Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
      RegisterWrite(LR_RegModemConfig2+(RFM96SpreadFactorTbl[gb_SF]<<4)+(CRC_EN<<2)+0x03);  //SFactor &  LNA gain set by the internal AGC loop 
    }


//	RegisterWrite(LR_RegModemConfig1+0x36); //BW=3 20.8KHz , CR=3 4/7, explit
//	RegisterWrite(LR_RegModemConfig2+0x77); //SF=6, CRC on


    RegisterWrite(LR_RegSymbTimeoutLsb+0xFF);                   //RegSymbTimeoutLsb Timeout = 0x3FF(Max) 
    
    RegisterWrite(LR_RegPreambleMsb + 0);                       //RegPreambleMsb 
    RegisterWrite(LR_RegPreambleLsb + 12);                      //RegPreambleLsb 8+4=12byte Preamble
    
    RegisterWrite(REG_LR_DIOMAPPING2+0x01);                     //RegDioMapping2 DIO5=00, DIO4=01
    RFM96_Standby();                                         //Entry standby mode
}

/**********************************************************
**Name:     RFM96_LoRaEntryRx
**Function: Entry Rx mode
**Input:    None
**Output:   None
**********************************************************/
void RFM96_LoRaEntryRx(void)
{
  uint8_t addr; 
        
  RFM96_Config(0);                                         //setting base parameter
  
  RegisterWrite(0x4D00+0x84);                                   //Normal and Rx
  RegisterWrite(LR_RegHopPeriod+0xFF);                          //RegHopPeriod NO FHSS
  RegisterWrite(REG_LR_DIOMAPPING1+0x01);                       //DIO0=00, DIO1=00, DIO2=00, DIO3=01  DIO0=00--RXDONE
      
  RegisterWrite(LR_RegIrqFlagsMask+0x3F);                       //Open RxDone interrupt & Timeout
  RFM96_LoRaClearIrq();   
  
  //TODO
  RegisterWrite(LR_RegPayloadLength+21);                       //RegPayloadLength  21byte(this register must difine when the data long of one byte in SF is 6)
    
  addr = RegisterRead((uint8_t)(LR_RegFifoRxBaseAddr>>8));           //Read RxBaseAddr
  RegisterWrite(LR_RegFifoAddrPtr+addr);                        //RxBaseAddr -> FiFoAddrPtr　 
  RegisterWrite(LR_RegOpMode+0x0D);                        //Continuous Rx Mode
  
}

/**********************************************************
**Name:     RFM96_LoRaRxWaitStable
**Function: Determine whether the state of stable Rx 查询RX 状态
**Input:    none
**Output:   none
**********************************************************/
uint8_t RFM96_LoRaRxWaitStable(void)
{ 
	uint8_t tmp;
	tmp=RegisterRead((uint8_t)(LR_RegModemStat>>8));
	return tmp;
}

/**********************************************************
**Name:     RFM96_LoRaRxPacket
**Function: Receive data in LoRa mode
**Input:    None
**Output:   1- Success
            0- Fail
**********************************************************/
uint8_t RFM96_LoRaRxPacket(uint8_t *buf)
{
		uint8_t i; 
		uint8_t addr;
		uint8_t packet_size;

	 
			
				for(i=0;i<32;i++) //清Buffer
					buf[i] = 0x00;

				addr = RegisterRead((uint8_t)(LR_RegFifoRxCurrentaddr>>8));      //last packet addr 数据包的最后地址(数据的尾地址)
				RegisterWrite(LR_RegFifoAddrPtr+addr);                      //RxBaseAddr -> FiFoAddrPtr   
				delay_us(1);

				if(RFM96SpreadFactorTbl[gb_SF]==6)           //When SpreadFactor is six，will used Implicit Header mode(Excluding internal packet length)
					packet_size=21;
				else
					packet_size = RegisterRead((uint8_t)(LR_RegRxNbBytes>>8));     //Number for received bytes    

				SPIBurstRead(0x00, buf, packet_size);

				RFM96_LoRaClearIrq();
				delay_us(1);
				 return packet_size;
		 
	 
}


/**********************************************************
**Name:     RFM96_LoRaEntryTx
**Function: Entry Tx mode
**Input:    None
**Output:   None
**********************************************************/
uint8_t RFM96_LoRaEntryTx(uint8_t packet_length)
{
		uint8_t addr;
		uint8_t SysTime = 0;
		uint8_t temp;

		RFM96_Config(0);                                         //模块发射参数设置
		delay_us(1000);
		RegisterWrite(0x4D00+0x87);                                   //发射功率 for 20dBm
		RegisterWrite(LR_RegHopPeriod);                               //RegHopPeriod NO FHSS
		RegisterWrite(REG_LR_DIOMAPPING1+0x41);                       //DIO0=01, DIO1=00, DIO2=00, DIO3=01

		RFM96_LoRaClearIrq();
		RegisterWrite(LR_RegIrqFlagsMask+0xF7);                       //Open TxDone interrupt
		RegisterWrite(LR_RegPayloadLength+packet_length);                       //RegPayloadLength  21byte负载和fifo的字节数的关系是什么？？

		addr = RegisterRead((uint8_t)(LR_RegFifoTxBaseAddr>>8));           //RegFiFoTxBaseAddr
		RegisterWrite(LR_RegFifoAddrPtr+addr);                        //RegFifoAddrPtr
		
	while(1)
	{
		temp=RegisterRead((uint8_t)(LR_RegPayloadLength>>8) );
		if(temp==packet_length)
		{
			break; 
		}
		if(SysTime>=3)	
			return 0;
	}
 
    return 	packet_length;
}

/**********************************************************
**Name:     RFM96_LoRaTxPacket
**Function: Send data in LoRa mode
**Input:    None
**Output:   1- Send over
**********************************************************/
uint8_t RFM96_LoRaTxPacket(uint8_t *buf,uint8_t len)
{ 
         
    BurstWrite(0x00, (uint8_t *)buf, len);  
    RegisterWrite(LR_RegOpMode+0x03+0x08);                    //Tx Mode       

    uint16_t count=0;	
    while( sx1276IsIRQPinAsserted() == false )
    {
        if(++count>1000)
        {
            break;
        }
        delay_ms(1);
    }

    RegisterRead((uint8_t)(LR_RegIrqFlags>>8));
    RFM96_LoRaClearIrq();                                //Clear irq
    RFM96_Standby();                                     //Entry Standby mode      

    if(count>1000)
    {
        return 0;
    }	
    return len;  
}







