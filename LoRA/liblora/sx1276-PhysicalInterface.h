


#ifndef __SX1276PHYSICALINTERFACE_H__
#define __SX1276PHYSICALINTERFACE_H__


#include <stdint.h>
#include <stdbool.h>
#include "SPIBus.h"


/**********************************************************
**Name:     sx1276RegisterRead
**Function: SPI Read CMD
**Input:    adr -> address for read
**Output:   None
**********************************************************/
uint8_t sx1276RegisterRead( SPISlaveID id, uint8_t adr);

/**********************************************************
**Name:     sx1276RegisterWrite
**Function: SPI Write CMD
**Input:    WrPara -> address & data
**Output:   None
**********************************************************/
void sx1276RegisterWrite( SPISlaveID id, uint16_t WrPara);

/**********************************************************
**Name:     sx1276BlockRead
**Function: SPI burst read mode
**Input:    adr-----address for read
**          ptr-----data buffer point for read
**          length--how many bytes for read
**Output:   None
**********************************************************/
void sx1276BlockRead( SPISlaveID id, uint8_t adr, uint8_t *ptr, uint8_t length);

/**********************************************************
**Name:     SPIsx1276BlockWrite
**Function: SPI burst write mode
**Input:    adr-----address for write
**          ptr-----data buffer point for write
**          length--how many bytes for write
**Output:   none
**********************************************************/
void sx1276BlockWrite( SPISlaveID id, uint8_t adr, uint8_t *ptr, uint8_t length);

void sx1276Reset( SPISlaveID id );

bool sx1276IsIRQPinAsserted( SPISlaveID id );

void SX1276SetReset( SPISlaveID id, uint8_t state );

void SX1276WriteBuffer( SPISlaveID id, uint8_t addr, uint8_t *buffer, uint8_t size );

void SX1276ReadBuffer( SPISlaveID id, uint8_t addr, uint8_t *buffer, uint8_t size );

void SX1276Write( SPISlaveID id, uint8_t addr, uint8_t data );

void SX1276Read( SPISlaveID id, uint8_t addr, uint8_t *data );

void SX1276SetIdleState( SPISlaveID id );

void SX1276WriteFifo( SPISlaveID id, uint8_t *buffer, uint8_t size );

void SX1276ReadFifo( SPISlaveID id, uint8_t *buffer, uint8_t size );

uint8_t SX1276ReadDio0(  SPISlaveID id  );

uint8_t SX1276ReadDio1(  SPISlaveID id  );

uint8_t SX1276ReadDio2(  SPISlaveID id  );

uint8_t SX1276ReadDio3(  SPISlaveID id  );

uint8_t SX1276ReadDio4(  SPISlaveID id  );

uint8_t SX1276ReadDio5(  SPISlaveID id  );

void SX1276WriteRxTx( SPISlaveID id, uint8_t txEnable );

void SX1276Reset(  SPISlaveID id  );

void sx1276PhysicalInterfaceInit(SPISlaveID id);

uint8_t SPICmd8bit(SPISlaveID id, uint8_t WrPara);


#endif

