


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



int     fd      = -1;

//
//
//
typedef struct
{
    uint32_t    timestamp;
    uint32_t    eventType;
    uint32_t    numberOfPayloadBytes;
    uint8_t     payload[1];

} Event;

typedef void (*EventNotificationHandler)(Event*);



//
//
//
void TransmitEvent( Event* event )
{
    write( fd, event, sizeof(*event) );
}


//
//
//
void RequestEventNotification( EventNotificationHandler handler )
{

}

//
//
//
bool PollForEvent( Event* event )
{
    bool    success                 = false;
    int     numberOfBytesAvailable  = 0;
    int     ioctlResult             = ioctl( fd, FIONREAD, &numberOfBytesAvailable );

    ValidatePointerForRW(event);

    if( ioctlResult != 0 )
    {
        //
        // ioctl failed on fd.
        //
        InternalFailure();
    }
    else
    {
        if(numberOfBytesAvailable > sizeof(Event) )
        {
            ssize_t bytesRead  = read( fd, event, sizeof(*event) );
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
void AuraInitialise()
{
    fd  = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY );
    if(fd == -1)
    {
        //
        // Couldn't open serial port.
        //
        InternalFailure();
    }

}


////////////////////////////////////////////////////////////////////////////////


void EventHandler(Event* event)
{
    printf( "<Event received>\n" );
}


//
//
//
int main()
{
    printf("Aura DeviceSide demo.\n");

    AuraInitialise();

    RequestEventNotification( EventHandler );

    //
    //
    //
    while(true)
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        struct timeval timeout = { 1, 0 }; /* 10 seconds */

        int ret = select(fd+1, &fds, NULL, NULL, &timeout);
        if (ret ==1)
        {
            Event   event;

            if(PollForEvent( &event ) == true)
            {
                printf("<EventReceived>\n");
            }

            uint8_t         rxData  = 0;
            read( fd, &rxData, 1 );
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
