//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, August 2016.
//



#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

#include "Halo.h"


////////////////////////////////////////////////////////////////////////////////


//
//
//
void InternalFailure()
{
    printf("<InternalFailure>\n");
    exit(-1);
}


//
//
//
void ValidatePointerForRW( void* pointer )
{
    if(pointer == NULL)
    {
        InternalFailure();
    }
}


//
//
//
uint32_t GetCurrentTimestamp()
{
    static uint32_t     timestamp   = 1000;
    timestamp++;

    return timestamp;
}


////////////////////////////////////////////////////////////////////////////////



int             haloFd              = -1;
uint32_t        numberOfBytesRead   = 0;



//
//
//
void HaloTransmitEvent( HaloEvent* event )
{
    //
    // Get the current timestamp for the event.
    //
    event->timestamp     = GetCurrentTimestamp();

    //
    // Write the event to the pipe.
    //
    int numberOfBytesWritten    = write( haloFd, event, sizeof(*event) );
    if( numberOfBytesWritten != sizeof(*event) )
    {
        //
        // We didn't manage to write a complete event to the pipe.
        //
        InternalFailure();
    }

    //
    //
    //
    int     ioctlResult     = tcflush( haloFd, TCIOFLUSH );
    if( ioctlResult != 0 )
    {
        //
        // ioctl failed on haloFd.
        //
        printf("tcflush != 0\n");
        perror("ioctl:");
        InternalFailure();
    }



    printf("<%d bytes written>\n", numberOfBytesWritten);
}


//
//
//
void HaloRequestEventNotification( EventNotificationHandler handler )
{

}

//
// #define TIOCM_LE    0x001
// #define TIOCM_DTR   0x002
// #define TIOCM_RTS   0x004
// #define TIOCM_ST    0x008
// #define TIOCM_SR    0x010
// #define TIOCM_CTS   0x020
// #define TIOCM_CAR   0x040
// #define TIOCM_RNG   0x080
// #define TIOCM_DSR   0x100
// #define TIOCM_CD    TIOCM_CAR
// #define TIOCM_RI    TIOCM_RNG
//
bool HaloPollForEvent( HaloEvent* event )
{
    bool    success                 = false;
    int     numberOfBytesAvailable  = 0;
    int     ioctlResult             = ioctl( haloFd, FIONREAD, &numberOfBytesAvailable );

    ValidatePointerForRW(event);

    int status  = 0;
    ioctl(haloFd, TIOCMGET, &status );
    printf("port status = %08x\n", status);

    if( ioctlResult != 0 )
    {
        //
        // ioctl failed on haloFd.
        //
        printf("ioctlResult != 0\n");
        perror("ioctl:");
        InternalFailure();
    }
    else
    {
        uint32_t            numberOfBytesToRead         = numberOfBytesAvailable;
        static uint8_t      eventData[sizeof(*event)];

        //
        // Decide how many bytes we need to read to complete the event.
        //
        numberOfBytesToRead     = sizeof(*event) - numberOfBytesRead;
        if(numberOfBytesToRead >= numberOfBytesAvailable)
        {
            numberOfBytesToRead     = numberOfBytesAvailable;
        }

        //
        // Attempt to read the bytes from the pipe.
        //
        ssize_t bytesRead  = read( haloFd, &eventData[numberOfBytesRead], numberOfBytesToRead );
        if(bytesRead != numberOfBytesToRead)
        {
            int status  = 0;
            ioctl(haloFd, TIOCMGET, &status );
            printf("port status = %08x\n", status);

            //
            // Couldn't read advertised number of bytes.
            //
            printf("bytesRead != numberOfBytesToRead (%d != %d, with %d)\n", bytesRead, numberOfBytesToRead, numberOfBytesAvailable);
            perror("read:");
            InternalFailure();
        }
        else
        {
            numberOfBytesRead   += numberOfBytesToRead;
        }

        //
        // If we've got the correct number of bytes for an event, lets indicate success.
        //
        if(numberOfBytesRead == sizeof(*event) )
        {
            memcpy( event, &eventData[0], sizeof(*event) );
            success             = true;
            numberOfBytesRead   = 0;
        }
    }

    return success;
}


//
//
//
void HaloInitialise()
{
    haloFd  = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY  | O_NDELAY );
    if(haloFd == -1)
    {
        //
        // Couldn't open serial port.
        //
        InternalFailure();
    }

}


////////////////////////////////////////////////////////////////////////////////


void HaloEventHandler( HaloEvent* event )
{
    printf( "<Event received>\n" );
}


//
//
//
int main()
{
    printf("Halo DeviceSide demo.\n");

    HaloInitialise();

    HaloRequestEventNotification( HaloEventHandler );

    //
    //
    //
    while(true)
    {
        fd_set haloFds;
        FD_ZERO(&haloFds);
        FD_SET(haloFd, &haloFds);
        struct timeval timeout = { 1, 0 }; /* 1 seconds */

        int ret = select(haloFd+1, &haloFds, NULL, NULL, &timeout);
        if (ret == 1)
        {
            HaloEvent   event;

            printf("<Getting event>\n");
            if(HaloPollForEvent( &event ) == true)
            {
                printf( "<EventReceived %08x %08x %08x>\n", event.timestamp, event.type, event.numberOfPayloadBytes );
                fflush(stdout);

                switch(event.type)
                {
                    case HALO_IDENTITY:
                    {
                        printf("<HALO_IDENTITY>\n");

                        static HaloEvent    identityReponseEvent     =
                        {
                            .timestamp              = 123,
                            .type                   = HALO_IDENTITY,
                            .numberOfPayloadBytes   = 123456789,
                        };
                        HaloTransmitEvent( &identityReponseEvent );

                        break;
                    }

                    default:
                    {
                        printf("<Unknown>\n");
                        HaloTransmitEvent( &event );
                        break;
                    }
                }
            }
            else
            {
                printf("%d/%d\n", numberOfBytesRead, sizeof(event) );
            }

        }
        else if(ret == 0)
        {
            perror("timeout error ");
            numberOfBytesRead   = 0;
            fflush(stdout);
        }
        else if (ret ==-1)
        {
            numberOfBytesRead   = 0;
            perror("some other error");
            fflush(stdout);
        }

    }

}
