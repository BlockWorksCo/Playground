

#ifndef __SPIBUS_H__
#define __SPIBUS_H__


#include <stdint.h>
#include <stdbool.h>



typedef enum
{
    SlaveA,
    SlaveB,

} SPISlaveID;



void spiBusInit();

void spiBusSelectSlave( SPISlaveID slaveID );

void spiBusDeselectSlave( SPISlaveID slaveID );

uint8_t spiBusWriteOneByte( uint8_t byte );

uint8_t spiBusReadOneByte();

#endif

