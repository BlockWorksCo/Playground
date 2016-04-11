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

    lseek(fd, RAW_DEVICE_SIZE-1, SEEK_SET);
    write(fd, "", 1);

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
    //printf("erasing page %d\n", page);
    lseek( fd, (uint32_t)page*PAGE_SIZE, SEEK_SET );
    off_t   pos     = lseek( fd, 0, SEEK_CUR );
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
    uint32_t    pageNumber              = offset / PAGE_SIZE; 
    uint32_t    offsetIntoPage          = offset % PAGE_SIZE;
    uint8_t     existingPage[PAGE_SIZE];

    //
    // Read...
    //
    lseek( fd, pageNumber*PAGE_SIZE, SEEK_SET );
    read( fd, &existingPage[0], PAGE_SIZE );

    //
    // ...modify
    //
    for(uint32_t i=0; i<numberOfBytes; i++)
    {
        existingPage[offsetIntoPage+i]  = data[i];
    }

    //
    // ...write.
    //
    lseek( fd, pageNumber*PAGE_SIZE, SEEK_SET );
    write( fd, &existingPage[0], PAGE_SIZE );

    return true;
}


void FLASHDeviceRead(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    off_t   pos = lseek( fd, offset, SEEK_SET );
    if(pos != offset)
    {
        //printf("pos %d != %d\n", (int)pos, offset );
        exit(-1);
    }
    read( fd, &data[0], numberOfBytes );
    
    //printf("-- Reading %d bytes from %08x\n", numberOfBytes, offset);
}


