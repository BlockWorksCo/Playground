


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>



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



int     haloFd      = -1;

//
//
//
typedef struct __attribute__ ((__packed__))
{
    uint32_t    timestamp;
    uint32_t    eventType;
    uint32_t    numberOfPayloadBytes;
    uint8_t     payload[1];

} HaloEvent;

typedef void (*EventNotificationHandler)(HaloEvent*);



//
//
//
void HaloTransmitEvent( HaloEvent* event )
{
    write( haloFd, event, sizeof(*event) );
}


//
//
//
void HaloRequestEventNotification( EventNotificationHandler handler )
{

}

//
//
//
bool HaloPollForEvent( HaloEvent* event )
{
    bool    success                 = false;
    int     numberOfBytesAvailable  = 0;
    int     ioctlResult             = ioctl( haloFd, FIONREAD, &numberOfBytesAvailable );

    ValidatePointerForRW(event);

    if( ioctlResult != 0 )
    {
        //
        // ioctl failed on haloFd.
        //
        InternalFailure();
    }
    else
    {
        if(numberOfBytesAvailable > sizeof(*event) )
        {
            ssize_t bytesRead  = read( haloFd, event, sizeof(*event) );
            if( bytesRead == sizeof(*event) )
            {
                success     = true;
            }
            else
            {
                //
                // Couldn't read advertised number of bytes.
                //
                InternalFailure();
            }
        }
    }

    return success;
}


//
//
//
void HaloInitialise()
{
    haloFd  = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY );
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
        struct timeval timeout = { 1, 0 }; /* 10 seconds */

        int ret = select(haloFd+1, &haloFds, NULL, NULL, &timeout);
        if (ret ==1)
        {
            HaloEvent   event;

            if(HaloPollForEvent( &event ) == true)
            {
                printf("<EventReceived>\n");
            }

            uint8_t         rxData  = 0;
            read( haloFd, &rxData, 1 );
            printf("%c", rxData);
        }
        else if(ret == 0)
        {
            perror("timeout error ");
        }
        else if (ret ==-1)
        {
            perror("some other error");
        }

    }

}
