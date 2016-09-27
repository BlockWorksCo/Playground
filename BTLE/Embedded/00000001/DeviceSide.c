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
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
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
        if(bytesRead < 0)
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
            numberOfBytesRead   += bytesRead;
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
    haloFd  = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY );
    if(haloFd == -1)
    {
        //
        // Couldn't open serial port.
        //
        InternalFailure();
    }

    //
    //
    //
    fcntl( haloFd, F_SETFL, FNDELAY);

}


////////////////////////////////////////////////////////////////////////////////


void HaloEventHandler( HaloEvent* event )
{
    printf( "<Event received>\n" );
}




void timer_handler (int signum)
{
    static int count = 0;
    printf ("--------  timer expired %d times -----------\n", ++count);



    HaloEvent    identityReponseEvent     =
    {
        .timestamp              = GetCurrentTimestamp(),
        .type                   = 345,
        .numberOfPayloadBytes   = 0xa500a500,
    };
    HaloTransmitEvent( &identityReponseEvent );


}




//
//
//
uint32_t    angle   = 0;

uint32_t    EditTimeHoursInput  = 0;
uint32_t    EditTimeMinsInput   = 0;
uint32_t    EditTimeSecsInput   = 0;
uint32_t    EditDistanceInput   = 0;
uint32_t    EditPaceMinsInput   = 0;
uint32_t    EditPaceSecsInput   = 0;

bool        timeModifiedFlag        = false;
bool        paceModifiedFlag        = false;
bool        distanceModifiedFlag    = false;

uint32_t    currentDistance         = 0;
uint32_t    currentTime             = 0;
uint32_t    currentPace             = (1*60) + 45;
uint32_t    currentSPM              = 30;
uint32_t    secondsIntoRun          = 0;

uint32_t    targetTime              = 0;
uint32_t    targetPace              = 0;
uint32_t    targetDistance          = 0;


void Go()
{
    if(timeModifiedFlag == true)
    {
        targetTime      = (EditTimeHoursInput*3600) + (EditTimeMinsInput*60) + EditTimeSecsInput;
    }
    else if(paceModifiedFlag == true)
    {
        targetPace      = (EditPaceMinsInput*60) + EditPaceSecsInput;
    }
    else if(distanceModifiedFlag == true)
    {
        targetDistance  = EditDistanceInput;
    }

    currentTime     = 0;
}



void UpdateSimulation()
{
    //
    //
    //
    uint32_t    metresPerStroke     = currentTime;


    currentTime++;

    //
    //
    //
    {
        HaloEvent    reponseEvent     =
        {
            .timestamp              = GetCurrentTimestamp(),
            .type                   = 11,
            .numberOfPayloadBytes   = currentDistance,
        };

        HaloTransmitEvent( &reponseEvent );            
    }

    {
        HaloEvent    reponseEvent     =
        {
            .timestamp              = GetCurrentTimestamp(),
            .type                   = 12,
            .numberOfPayloadBytes   = currentTime,
        };

        HaloTransmitEvent( &reponseEvent );            
    }

    {
        HaloEvent    reponseEvent     =
        {
            .timestamp              = GetCurrentTimestamp(),
            .type                   = 13,
            .numberOfPayloadBytes   = currentPace,
        };

        HaloTransmitEvent( &reponseEvent );            
    }

    {
        HaloEvent    reponseEvent     =
        {
            .timestamp              = GetCurrentTimestamp(),
            .type                   = 14,
            .numberOfPayloadBytes   = currentSPM,
        };

        HaloTransmitEvent( &reponseEvent );            
    }
}


//
//
//
int main()
{
    printf("Halo DeviceSide demo.\n");

    HaloInitialise();

    HaloRequestEventNotification( HaloEventHandler );


    /* Install timer_handler as the signal handler for SIGVTALRM. */
    struct sigaction sa;
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGALRM, &sa, NULL);

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

                        HaloEvent    identityReponseEvent     =
                        {
                            .timestamp              = GetCurrentTimestamp(),
                            .type                   = HALO_IDENTITY,
                            .numberOfPayloadBytes   = 0x00000000,
                        };
                        HaloTransmitEvent( &identityReponseEvent );

                        break;
                    }

                    case 2:
                    {
                        printf("<HALO_MSG 2>\n");


                        /* Configure the timer to expire after 250 msec... */
                        static struct itimerval timer;
                        timer.it_value.tv_sec = 5;
                        timer.it_value.tv_usec = 0;
                        /* ... and every 250 msec after that. */
                        timer.it_interval.tv_sec = 0;
                        timer.it_interval.tv_usec = 0;
                        /* Start a virtual timer. It counts down whenever this process is
                        executing. */
                        setitimer (ITIMER_REAL, &timer, NULL);

                        break;
                    }

                    case 3:
                    {
                        printf("EditTimeHoursInput : %08x\n", event.numberOfPayloadBytes);
                        EditTimeHoursInput  = event.numberOfPayloadBytes;
                        timeModifiedFlag    = true;
                        break;
                    }

                    case 4:
                    {
                        printf("EditTimeMinsInput : %08x\n", event.numberOfPayloadBytes);
                        EditTimeMinsInput  = event.numberOfPayloadBytes;
                        timeModifiedFlag    = true;
                        break;
                    }

                    case 5:
                    {
                        printf("EditTimeSecsInput : %08x\n", event.numberOfPayloadBytes);
                        EditTimeSecsInput  = event.numberOfPayloadBytes;
                        timeModifiedFlag    = true;
                        break;
                    }

                    case 6:
                    {
                        printf("EditDistanceInput : %08x\n", event.numberOfPayloadBytes);
                        EditDistanceInput  = event.numberOfPayloadBytes;
                        distanceModifiedFlag    = true;
                        break;
                    }

                    case 7:
                    {
                        printf("EditPaceMinsInput : %08x\n", event.numberOfPayloadBytes);
                        EditPaceMinsInput  = event.numberOfPayloadBytes;
                        paceModifiedFlag    = true;
                        break;
                    }

                    case 8:
                    {
                        printf("EditPaceSecsInput : %08x\n", event.numberOfPayloadBytes);
                        EditPaceSecsInput  = event.numberOfPayloadBytes;
                        paceModifiedFlag    = true;
                        break;
                    }

                    case 9:
                    {
                        printf("\nGO!\n");
                        break;
                    }

                    default:
                    {
                        printf("<Unknown>\n");
                        HaloEvent    reponseEvent     =
                        {
                            .timestamp              = GetCurrentTimestamp(),
                            .type                   = 456,
                            .numberOfPayloadBytes   = angle,
                        };
                        HaloTransmitEvent( &reponseEvent );

                        angle   += 10;

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
            UpdateSimulation();

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
