//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//



#ifndef __FLASHDEVICE_H__
#define __FLASHDEVICE_H__


#include <stdint.h>
#include <stdbool.h>

#define PAGE_SIZE           (4096)

#define RAW_DEVICE_SIZE     (8*1024*1024)
#define NUMBER_OF_PAGES     (RAW_DEVICE_SIZE/PAGE_SIZE)




void FLASHDeviceInitialise();
bool FLASHDeviceErase(uint32_t page);
bool FLASHDeviceWrite(uint32_t offset, uint32_t numberOfBytes, uint8_t* data);
void FLASHDeviceRead(uint32_t offset, uint32_t numberOfBytes, uint8_t* data);

#endif

