//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//


#include "FLASHDevice.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



int     fd  = -1;


void FLASHDeviceInitialise()
{
    fd  = creat("FLASH.bin", O_RDWR|S_IRWXU);
    if(fd == -1)
    {
        exit(-1);
    }

    FLASHDeviceEraseDevice();
}


bool FLASHDeviceEraseDevice()
{
    for(uint32_t i=0; i<NUMBER_OF_PAGES; i++)
    {
        FLASHDeviceErasePage( i );        
    }

    return true;
}


bool FLASHDeviceErasePage(uint32_t page)
{
    lseek( fd, SEEK_SET, page*PAGE_SIZE );

    uint8_t     data[PAGE_SIZE];
    memset(&data[0], 0xff, PAGE_SIZE);

    ssize_t result = write( fd, &data[0], PAGE_SIZE );
    if(result != PAGE_SIZE)
    {
        printf("<Cant write to FLASH!>\n");
        exit(-1);
        return false;
    }

    return true;
}


bool FLASHDeviceWrite(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    lseek( fd, SEEK_SET, offset );
    write( fd, &data[0], numberOfBytes );

    return true;
}


void FLASHDeviceRead(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    lseek( fd, SEEK_SET, offset );
    read( fd, &data[0], numberOfBytes );
}


