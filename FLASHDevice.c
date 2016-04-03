

#include "FLASHDevice.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


int     fd  = -1;


void FLASHDeviceInitialise()
{
    fd  = creat("FLASH.bin", O_RDWR|S_IRWXU);
    if(fd == -1)
    {
        exit(-1);
    }

    for(uint32_t i=0; i<NUMBER_OF_PAGES; i++)
    {
        FLASHDeviceErase( i );        
    }
}


bool FLASHDeviceErase(uint32_t page)
{
    lseek( fd, SEEK_SET, page*PAGE_SIZE );

    uint8_t     data[PAGE_SIZE];
    write( fd, &data[0], PAGE_SIZE );

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


