


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

void sx1276PhysicalInterfaceInit(SPISlaveID id);


#endif

