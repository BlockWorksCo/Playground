

#ifndef __ETHERNET_H__
#define __ETHERNET_H__


#include "Common.h"
#include "SPIMaster.h"

typedef SPISlaveID  EthernetID;


void ethernetInit( EthernetID id );

bool ethernetPollForPacket( EthernetID id, uint8_t** packet, size_t* packetSize );

bool ethernetTransmitPacket( EthernetID id,  uint8_t* packet, size_t packetSize );


#endif



