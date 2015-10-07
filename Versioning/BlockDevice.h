

#ifndef __BLOCKDEVICE_H__
#define __BLOCKDEVICE_H__

#include <stdint.h>


#define BLOCKDEVICE_SIZE        (1024*16)




void BlockDeviceInitialise();
void BlockDeviceRead(uint32_t offset, uint32_t numberOfBytes, uint8_t* data);
void BlockDeviceWrite(uint32_t offset, uint32_t numberOfBytes, uint8_t* data);

#endif




