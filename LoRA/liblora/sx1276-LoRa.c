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
#include <stdio.h>
#include <string.h>




#define gb_SF  3
#define gb_BW  6
#define CR    0x04
#define CRC_EN   0x00  //CRC Enable


//
//
//
bool transmitInProgress_SlaveA  = false;
bool transmitInProgress_SlaveB  = false;


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

const uint8_t RFM96SpreadFactorTbl[6] =
{
    7,8,9,10,11,12
};

const uint8_t RFM96LoRaBwTbl[10] =
{   // 0        1     2       3      4       5         6      7       8      9
//7.8KHz,10.4KHz,15.6KHz,20.8KHz,31.2KHz,41.7KHz,62.5KHz,125KHz,250KHz,500KHz
    0,1,2,3,4,5,6,7,8,9
};




/**********************************************************
**Name:     loraStandbyMode
**Function: Entry standby mode
**Input:    None
**Output:   None
**********************************************************/
void loraStandbyMode( SPISlaveID id )
{
    sx1276RegisterWrite( id, LR_RegOpMode+0x01+0x08);                              //Standby
}

/**********************************************************
**Name:     loraSleepMode
**Function: Entry sleep mode
**Input:    None
**Output:   None
**********************************************************/
void loraSleepMode( SPISlaveID id )
{
    sx1276RegisterWrite( id, LR_RegOpMode+0x00+0x08);                              //Sleep
}

/*********************************************************/
//LoRa mode
/*********************************************************/
/**********************************************************
**Name:     loraLoraMode
**Function: Set RFM69 entry LoRa(LongRange) mode
**Input:    None
**Output:   None
**********************************************************/
void loraLoraMode( SPISlaveID id )
{
    sx1276RegisterWrite( id, LR_RegOpMode+0x80+0x08);
}

/**********************************************************
**Name:     loraClearAllIRQFlags
**Function: Clear all irq
**Input:    None
**Output:   None
**********************************************************/
void loraClearAllIRQFlags( SPISlaveID id )
{
    sx1276RegisterWrite( id, LR_RegIrqFlags+0xFF);
}

/**********************************************************
**Name:     loraBasicConfiguration
**Function: RFM96 base config
**Input:    mode
**Output:   None
**********************************************************/
void loraBasicConfiguration( SPISlaveID id, uint8_t mode)
{
    uint8_t i;

    sx1276Reset(id);
    loraSleepMode(id);                                           //Change modem mode Must in Sleep mode
    delay_ms(1);
    loraLoraMode(id);

    for(i=0; i<3; i++)                                     //setting frequency parameter
    {
        sx1276RegisterWrite( id, RFM96FreqTbl[i]);
    }

    //setting base parameter
    sx1276RegisterWrite( id, RFM96PowerTbl[0]);             //Setting output power parameter
    sx1276RegisterWrite( id, LR_RegOcp+0x0B);                              //RegOcp,Close Ocp
    sx1276RegisterWrite( id, LR_RegLna+0x23);                              //RegLNA,High & LNA Enable

    if(RFM96SpreadFactorTbl[gb_SF]==6)           //SFactor=6
    {
        uint8_t tmp;
        sx1276RegisterWrite( id, LR_RegModemConfig1+(RFM96LoRaBwTbl[gb_BW]<<4)+(CR<<1)+0x01);//带宽设置，包结构，Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
        sx1276RegisterWrite( id, LR_RegModemConfig2+(RFM96SpreadFactorTbl[gb_SF]<<4)+(CRC_EN<<2)+0x03);
        tmp = sx1276RegisterRead( id, 0x31);
        tmp &= 0xF8;
        tmp |= 0x05;
        sx1276RegisterWrite( id, 0x3100+tmp);
        sx1276RegisterWrite( id, 0x3700+0x0C);
    }
    else
    {
        sx1276RegisterWrite( id, LR_RegModemConfig1+(RFM96LoRaBwTbl[gb_BW]<<4)+(CR<<1)+0x00);//Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
        sx1276RegisterWrite( id, LR_RegModemConfig2+(RFM96SpreadFactorTbl[gb_SF]<<4)+(CRC_EN<<2)+0x03);  //SFactor &  LNA gain set by the internal AGC loop
    }

    sx1276RegisterWrite( id, LR_RegSymbTimeoutLsb+0xFF);                   //RegSymbTimeoutLsb Timeout = 0x3FF(Max)

    sx1276RegisterWrite( id, LR_RegPreambleMsb + 0);                       //RegPreambleMsb
    sx1276RegisterWrite( id, LR_RegPreambleLsb + 12);                      //RegPreambleLsb 8+4=12byte Preamble

    sx1276RegisterWrite( id, REG_LR_DIOMAPPING2_LONG+0x01);                     //RegDioMapping2 DIO5=00, DIO4=01
    loraStandbyMode(id);                                         //Entry standby mode
}

/**********************************************************
**Name:     loraContinuousReceiveMode
**Function: Entry Rx mode
**Input:    None
**Output:   None
**********************************************************/
void loraContinuousReceiveMode( SPISlaveID id )
{
    uint8_t addr;

    loraBasicConfiguration(id,0);                                         //setting base parameter

    sx1276RegisterWrite( id, 0x4D00+0x84);                                   //Normal and Rx
    sx1276RegisterWrite( id, LR_RegHopPeriod+0xFF);                          //RegHopPeriod NO FHSS
    sx1276RegisterWrite( id, REG_LR_DIOMAPPING1_LONG+0x01);                       //DIO0=00, DIO1=00, DIO2=00, DIO3=01  DIO0=00--RXDONE

    sx1276RegisterWrite( id, LR_RegIrqFlagsMask+0x3F);                       //Open RxDone interrupt & Timeout
    loraClearAllIRQFlags(id);

    //TODO
    sx1276RegisterWrite( id, LR_RegPayloadLength+21);                       //RegPayloadLength  21byte(this register must difine when the data long of one byte in SF is 6)

    addr = sx1276RegisterRead( id, (uint8_t)(LR_RegFifoRxBaseAddr>>8));           //Read RxBaseAddr
    sx1276RegisterWrite( id, LR_RegFifoAddrPtr+addr);                        //RxBaseAddr -> FiFoAddrPtr　
    sx1276RegisterWrite( id, LR_RegOpMode+0x0D);                        //Continuous Rx Mode
}



/**********************************************************
**Name:     RFM96_LoRaRxPacket
**Function: Receive data in LoRa mode
**Input:    None
**Output:   1- Success
            0- Fail
**********************************************************/
uint8_t RFM96_LoRaRxPacket(SPISlaveID id, uint8_t *buf)
{
    uint8_t addr;
    uint8_t packet_size;

    addr = sx1276RegisterRead( id, (uint8_t)(LR_RegFifoRxCurrentaddr>>8));      //last packet addr 数据包的最后地址(数据的尾地址)
    sx1276RegisterWrite( id, LR_RegFifoAddrPtr+addr);                      //RxBaseAddr -> FiFoAddrPtr
    delay_us(1);

    if(RFM96SpreadFactorTbl[gb_SF]==6)           //When SpreadFactor is six，will used Implicit Header mode(Excluding internal packet length)
    {
        packet_size=21;
    }
    else
    {
        packet_size = sx1276RegisterRead( id, (uint8_t)(LR_RegRxNbBytes>>8));     //Number for received bytes
    }

    sx1276BlockRead(id, 0x00, buf, packet_size);

    loraClearAllIRQFlags(id);
    delay_us(1);

    return packet_size;
}



uint8_t loraTransmitPacket_Async(SPISlaveID id, uint8_t *buf,uint8_t len)
{
    sx1276BlockWrite(id, 0x00, (uint8_t *)buf, len);
    sx1276RegisterWrite( id, LR_RegOpMode+0x03+0x08);                    //Tx Mode

    if(id == SlaveA)
    {
        transmitInProgress_SlaveA   = true;
    }

    if(id == SlaveB)
    {
        transmitInProgress_SlaveB   = true;
    }

    return len;
}


//
// return value is true if a transmit can be started.
//
bool loraCheckAsyncTransmitForCompletion(SPISlaveID id)
{
    if((id == SlaveA) && (transmitInProgress_SlaveA == false)) 
    {
        // Transmit is not in progress, so indicate that a transmit
        // is allowed to be setup.
        return true;
    }

    if((id == SlaveB) && (transmitInProgress_SlaveB == false)) 
    {
        // Transmit is not in progress, so indicate that a transmit
        // is allowed to be setup.
        return true;
    }

    if( sx1276IsIRQPinAsserted(id) == true ) 
    {
        // Previous transmit is now complete, clear the IRQ flags and
        // move into standby mode.
        sx1276RegisterRead( id, (uint8_t)(LR_RegIrqFlags>>8));
        loraClearAllIRQFlags(id);                                //Clear irq
        loraContinuousReceiveMode( id );

        if(id == SlaveA) 
        {
            transmitInProgress_SlaveA   = false;
        }
        if(id == SlaveB) 
        {
            transmitInProgress_SlaveB   = false;
        }

        return true;
    }


    return false;
}



bool loraCheckAsyncReceiveCompletion(SPISlaveID id)
{
    if((id == SlaveA) && (transmitInProgress_SlaveA == true)) 
    {
        // If a transmit is in progress, we're not receiving.
        return false;
    }

    if((id == SlaveB) && (transmitInProgress_SlaveB == true)) 
    {
        // If a transmit is in progress, we're not receiving.
        return false;
    }

    if ( sx1276IsIRQPinAsserted(id) == true )
    {
        // receive-complete has been signalled.
        return true;
    }

    return false;
}





uint8_t loraReceivePacket( SPISlaveID id, uint8_t* buf, size_t maxBytesToReceive )
{
    uint8_t length = 0;
    
    memset( &buf[0], 0xff, maxBytesToReceive );
    length = RFM96_LoRaRxPacket( id, &buf[0] );
    
    loraContinuousReceiveMode( id );
    
    return length;
}   


void loraTransmitPacket( SPISlaveID id, uint8_t* buf, uint8_t size )
{
    uint8_t addr;
    uint8_t temp    = 0;

    loraBasicConfiguration(id, 0);                                         //模块发射参数设置
    sx1276RegisterWrite( id, 0x4D00+0x87);                                   //发射功率 for 20dBm
    sx1276RegisterWrite( id, LR_RegHopPeriod);                               //RegHopPeriod NO FHSS
    sx1276RegisterWrite( id, REG_LR_DIOMAPPING1_LONG+0x41);                       //DIO0=01, DIO1=00, DIO2=00, DIO3=01

    loraClearAllIRQFlags(id);
    sx1276RegisterWrite( id, LR_RegIrqFlagsMask+0xF7);                       //Open TxDone interrupt
    sx1276RegisterWrite( id, LR_RegPayloadLength+size);                       //RegPayloadLength  21byte负载和fifo的字节数的关系是什么？？

    addr = sx1276RegisterRead( id, (uint8_t)(LR_RegFifoTxBaseAddr>>8));           //RegFiFoTxBaseAddr
    sx1276RegisterWrite( id, LR_RegFifoAddrPtr+addr);                        //RegFifoAddrPtr

    while(temp != size)
    {
        temp = sx1276RegisterRead( id, (uint8_t)(LR_RegPayloadLength>>8) );
    }

    loraTransmitPacket_Async(id, buf,size);
}   



