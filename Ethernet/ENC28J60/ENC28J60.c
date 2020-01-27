

#include "Ethernet.h"
#include "SPIMaster.h"
#include "BoardSupport.h"
#include "Assertions.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



typedef enum
{
    ReadControlRegister     = 0x00,
    ReadBufferMemory        = 0x01,
    WriteControlRegister    = 0x02,
    WriteBufferMemory       = 0x03,
    BitFieldSet             = 0x04,
    BitFieldClear           = 0x05,
    SystemCommand           = 0x07,

} OpCode;




void enc28j60ReadOperation( EthernetID id, OpCode opCode, uint8_t argument, uint8_t* data, size_t numberOfBytes )
{
    if( opCode == ReadBufferMemory ) {
        argument    = 0x1a;
    }

    if( opCode == WriteBufferMemory ) {
        argument    = 0x1a;
    }

    if( opCode == SystemCommand ) {
        argument    = 0x1f;
    }

    // Form the command byte.
    uint8_t byte0   = (opCode<<5) | (argument&0x1ff);

    // Output to the device.
    spiBusSelectSlave( id );

    spiBusWriteOneByte(byte0);
    for(uint32_t i=0; i<numberOfBytes; i++) {
        data[i] = spiBusReadOneByte();
    }

    spiBusDeselectSlave( id );
}


void enc28j60WriteOperation( EthernetID id, OpCode opCode, uint8_t argument, uint8_t* data, size_t numberOfBytes )
{
    if( opCode == ReadBufferMemory ) {
        argument    = 0x1a;
    }

    if( opCode == WriteBufferMemory ) {
        argument    = 0x1a;
    }

    if( opCode == SystemCommand ) {
        argument    = 0x1f;
    }

    // Form the command byte.
    uint8_t byte0   = (opCode<<5) | (argument&0x1ff);

    // Output to the device.
    spiBusSelectSlave( id );

    spiBusWriteOneByte(byte0);
    for(uint32_t i=0; i<numberOfBytes; i++) {
        spiBusWriteOneByte(data[i]);
    }

    spiBusDeselectSlave( id );
}





void enc28J60WriteECON1( EthernetID id, uint8_t value )
{
    enc28j60WriteOperation( id, WriteControlRegister, 0x1F, &value, 1 );
}


void enc28J60SelectBank( EthernetID id, uint8_t bankId )
{
    uint8_t value   = 0x00;
    enc28j60ReadOperation( id, WriteControlRegister, 0x1F, &value, 1 );

    value   &= ~0x03;
    value   |= (bankId & 0x03);
    enc28j60WriteOperation( id, WriteControlRegister, 0x1F, &value, 1 );
}

uint16_t enc28j60ReadPHYRegister( EthernetID id, uint8_t registerId )
{
    // Set MIREGADR
    enc28J60SelectBank( id, 2 );
    enc28j60WriteOperation( id, WriteControlRegister, 0x14, &registerId, 1 );

    // Set MICMD.MIIRD.
    uint8_t temp    = 0x01;
    enc28j60WriteOperation( id, WriteControlRegister, 0x12, &temp, 1 );

    // Wait for MISTAT.BUSY
    Delay_ms(1);
    enc28J60SelectBank( id, 3 );
    temp    = 0x00;
    enc28j60ReadOperation( id, ReadControlRegister, 0x0a, &temp, 1 );

    // Clear MICMD.MIIRD
    temp    = 0x00;
    enc28J60SelectBank( id, 2 );
    enc28j60WriteOperation( id, WriteControlRegister, 0x12, &temp, 1 );

    // Read MIRDH
    uint8_t highByte    = 0x00;
    enc28j60ReadOperation( id, ReadControlRegister, 0x19, &highByte, 1 );

    // Read MIRDL
    uint8_t lowByte    = 0x00;
    enc28j60ReadOperation( id, ReadControlRegister, 0x18, &lowByte, 1 );

    // Combine.
    uint16_t    value   = (highByte << 8) | lowByte;

    return value;
}


void ethernetInit( EthernetID id )
{
    //
    GPIO_InitTypeDef GPIO_InitStructure;

    if( id == SlaveA ) 
    {
        // Set RESET pin to 0
        GPIO_WriteBit( GPIOB, GPIO_Pin_12, Bit_RESET );
        // Configure RESET as output
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
        GPIO_Init( GPIOB, &GPIO_InitStructure );

        GPIOB->ODR &= ~GPIO_Pin_12;
    
        Delay_ms(1);

        // Configure RESET as input
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
        GPIO_Init( GPIOB, &GPIO_InitStructure );

        GPIOB->ODR |= GPIO_Pin_12;
    }

    if( id == SlaveB ) 
    {
        // Set RESET pin to 0
        GPIO_WriteBit( GPIOB, GPIO_Pin_13, Bit_RESET );
        // Configure RESET as output
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
        GPIO_Init( GPIOB, &GPIO_InitStructure );

        GPIOB->ODR &= ~GPIO_Pin_13;

        Delay_ms(1);

        // Configure RESET as input
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
        GPIO_Init( GPIOB, &GPIO_InitStructure );

        GPIOB->ODR |= GPIO_Pin_13;
    }

    Delay_ms(10);

    //
    uint16_t    phid1   = enc28j60ReadPHYRegister( id, 0x02 );
    uint16_t    phid2   = enc28j60ReadPHYRegister( id, 0x03 );

    AssertThat( phid1 == 0x0083, "phid1" );
    AssertThat( phid2 == 0x0083, "phid2" );
}


bool ethernetPollForPacket( EthernetID id, uint8_t** packet, size_t* packetSize )
{
    return false;
}


bool ethernetTransmitPacket( EthernetID id, uint8_t* packet, size_t packetSize )
{
    return false;
}





