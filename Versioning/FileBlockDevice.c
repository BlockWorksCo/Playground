

#include "BlockDevice.h"
#include "ErrorHandler.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>



//
//
//
static const char*     fileName    = "FileBlockDevice.bin";

//
//
//
void BlockDeviceInitialise()
{
    int     fileHandle  = open(fileName, O_RDWR|O_CREAT|O_TRUNC, 0666 );

    if(fileHandle == -1)
    {
        PANIC("Couldn't open file. 1");
    }
    else
    {
        uint8_t     value   = 0xff;
        for(uint32_t i=0; i<BLOCKDEVICE_SIZE; i++)
        {
            write(fileHandle, &value, 1);
        }

        close(fileHandle);
    }
}


//
//
//
void BlockDeviceRead(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    int     fileHandle  = open(fileName, O_RDWR );

    if(fileHandle == -1)
    {
        PANIC("Couldn't open file. 2");
    }
    else
    {
        lseek( fileHandle, offset, SEEK_SET );
        read(fileHandle, data, numberOfBytes );

        close(fileHandle);
    }
}


//
//
//
void BlockDeviceWrite(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    int     fileHandle  = open(fileName, O_RDWR );

    if(fileHandle == -1)
    {
        PANIC("Couldn't open file. 3");
    }
    else
    {
        lseek( fileHandle, offset, SEEK_SET );
        write(fileHandle, data, numberOfBytes );

        close(fileHandle);
    }
}






