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
    fd  = open("FLASH.bin", O_RDWR|O_CREAT, S_IRWXU);
    if(fd == -1)
    {
        perror("Couldn't create file.\n");
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
    off_t   pos     = lseek( fd, SEEK_SET, page*PAGE_SIZE );
    if(pos != page*PAGE_SIZE)
    {
        printf("pos %d != %d\n", (int)pos, page*PAGE_SIZE);
        //exit(-1);
    }

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
    off_t   pos = lseek( fd, SEEK_SET, offset );
    if(pos != offset)
    {
        printf("pos %d != %d\n", (int)pos, offset );
        exit(-1);
    }
    write( fd, &data[0], numberOfBytes );

    printf("Writing %d bytes to %08x\n", numberOfBytes, offset );

    return true;
}


void FLASHDeviceRead(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    off_t   pos = lseek( fd, SEEK_SET, offset );
    if(pos != offset)
    {
        printf("pos %d != %d\n", (int)pos, offset );
        exit(-1);
    }
    read( fd, &data[0], numberOfBytes );
    
    printf("Reading %d bytes from %08x\n", numberOfBytes, offset);
}


