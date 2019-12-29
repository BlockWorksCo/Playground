


#ifndef __SX1276PHYSICALINTERFACE_H__
#define __SX1276PHYSICALINTERFACE_H__


#include <stdint.h>
#include <stdbool.h>
#include "SPIBus.h"


/**********************************************************
**Name:     RegisterRead
**Function: SPI Read CMD
**Input:    adr -> address for read
**Output:   None
**********************************************************/
uint8_t RegisterRead( SPISlaveID id, uint8_t adr);

/**********************************************************
**Name:     RegisterWrite
**Function: SPI Write CMD
**Input:    WrPara -> address & data
**Output:   None
**********************************************************/
void RegisterWrite( SPISlaveID id, uint16_t WrPara);

/**********************************************************
**Name:     SPIBurstRead
**Function: SPI burst read mode
**Input:    adr-----address for read
**          ptr-----data buffer point for read
**          length--how many bytes for read
**Output:   None
**********************************************************/
void SPIBurstRead( SPISlaveID id, uint8_t adr, uint8_t *ptr, uint8_t length);

/**********************************************************
**Name:     SPIBurstWrite
**Function: SPI burst write mode
**Input:    adr-----address for write
**          ptr-----data buffer point for write
**          length--how many bytes for write
**Output:   none
**********************************************************/
void BurstWrite( SPISlaveID id, uint8_t adr, uint8_t *ptr, uint8_t length);

void sx1276Reset();

bool sx1276IsIRQPinAsserted();

void SX1276SetReset( uint8_t state );

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );

void SX1276Write( uint8_t addr, uint8_t data );

void SX1276Read( uint8_t addr, uint8_t *data );

void SX1276SetIdleState(void);

void SX1276WriteFifo( uint8_t *buffer, uint8_t size );

void SX1276ReadFifo( uint8_t *buffer, uint8_t size );

uint8_t SX1276ReadDio0( void );

uint8_t SX1276ReadDio1( void );

uint8_t SX1276ReadDio2( void );

uint8_t SX1276ReadDio3( void );

uint8_t SX1276ReadDio4( void );

uint8_t SX1276ReadDio5( void );

void SX1276WriteRxTx( uint8_t txEnable );

void SX1276Reset( void );

void sx1276PhysicalInterfaceInit();

uint8_t SPICmd8bit(uint8_t WrPara);


#endif

