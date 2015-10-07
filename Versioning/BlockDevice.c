

#include "BlockDevice.h"
#include <string.h>


static uint8_t  BlockDeviceData[BLOCKDEVICE_SIZE];



//
//
//
void BlockDeviceInitialise()
{
    memset( &BlockDeviceData[0], 0xff, sizeof(BlockDeviceData) );
}


//
//
//
void BlockDeviceRead(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    memcpy( &data[0], &BlockDeviceData[offset], numberOfBytes );
}


//
//
//
void BlockDeviceWrite(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    memcpy( &BlockDeviceData[offset], &data[0], numberOfBytes );
}






