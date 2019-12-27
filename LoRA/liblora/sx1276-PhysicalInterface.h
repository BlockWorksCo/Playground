


#ifndef __SX1276PHYSICALINTERFACE_H__
#define __SX1276PHYSICALINTERFACE_H__


#include <stdint.h>
#include <stdbool.h>


/**********************************************************
**Name:     RegisterRead
**Function: SPI Read CMD
**Input:    adr -> address for read
**Output:   None
**********************************************************/
uint8_t RegisterRead(uint8_t adr);

/**********************************************************
**Name:     RegisterWrite
**Function: SPI Write CMD
**Input:    WrPara -> address & data
**Output:   None
**********************************************************/
void RegisterWrite(uint16_t WrPara);

/**********************************************************
**Name:     SPIBurstRead
**Function: SPI burst read mode
**Input:    adr-----address for read
**          ptr-----data buffer point for read
**          length--how many bytes for read
**Output:   None
**********************************************************/
void SPIBurstRead(uint8_t adr, uint8_t *ptr, uint8_t length);

/**********************************************************
**Name:     SPIBurstWrite
**Function: SPI burst write mode
**Input:    adr-----address for write
**          ptr-----data buffer point for write
**          length--how many bytes for write
**Output:   none
**********************************************************/
void BurstWrite(uint8_t adr, uint8_t *ptr, uint8_t length);

void sx1276Reset();

bool sx1276IsIRQPinAsserted();

#endif

