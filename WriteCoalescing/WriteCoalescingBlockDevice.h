

#ifndef __WRITECOALESCINGBLOCKDEVICE_H__
#define __WRITECOALESCINGBLOCKDEVICE_H__

#include "BlockDevice.h"




void WriteCoalescingBlockDeviceInitialise();
void WriteCoalescingBlockDeviceRead(uint32_t offset, uint32_t numberOfBytes, uint8_t* data);
void WriteCoalescingBlockDeviceWrite(uint32_t offset, uint32_t numberOfBytes, uint8_t* data);
void WriteCoalescingBlockDeviceFlush();


#endif




