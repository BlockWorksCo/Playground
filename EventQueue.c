//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>




// Utilities --------------------------------------------------------------------------------

void PANIC()
{
    printf("PANIC!\n");
}

uint32_t CurrentTimestamp()
{
    clock_t     value   = clock();
    const uint32_t  clocksPerMs     = CLOCKS_PER_SEC/1000;

    return value/clocksPerMs;
}









// EventQueue --------------------------------------------------------------------------------

#include "Queue.h"

typedef void (*Handler)();


DECLARE_QUEUE( HandlerQueue, Handler, 16);


void Call( Handler handler )
{
    HandlerQueuePut( handler );
}


void EventLoop()
{
    Handler     handler;

    do
    {
        handler = HandlerQueueGet( 0 );
        if( handler != 0 )
        {
            handler();
        }

    } while( handler != 0 );
}








// TimedEventQueue --------------------------------------------------------------------------------


#include "Utilities.h"


typedef struct
{
    Handler     handler;
    uint32_t    interval;
    uint32_t    firingTime;

} TimedEventHandler;

TimedEventHandler   timedEventHandlers[8];


void CallEvery_ms( Handler handler, uint32_t interval )
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(timedEventHandlers); i++)
    {
        if( timedEventHandlers[i].handler == 0 )
        {
            timedEventHandlers[i].handler       = handler;
            timedEventHandlers[i].interval      = interval;
            timedEventHandlers[i].firingTime    = CurrentTimestamp() + interval;
            break;
        }

    }
}

void CallAfter_ms( Handler handler, uint32_t interval )
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(timedEventHandlers); i++)
    {
        if( timedEventHandlers[i].handler == 0 )
        {
            timedEventHandlers[i].handler       = handler;
            timedEventHandlers[i].interval      = 0;
            timedEventHandlers[i].firingTime    = CurrentTimestamp() + interval;
            break;
        }

    }    
}


void CheckTimedEventHandlers()
{
    uint32_t    timestamp   = CurrentTimestamp();

    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(timedEventHandlers); i++)
    {
        if( timedEventHandlers[i].handler != 0 )
        {
            if( timestamp >= timedEventHandlers[i].firingTime )
            {
                Call( timedEventHandlers[i].handler );

                if(timedEventHandlers[i].interval == 0)
                {
                    timedEventHandlers[i].handler   = 0;
                }
                else
                {
                    timedEventHandlers[i].firingTime    = timestamp + timedEventHandlers[i].interval;                    
                }
            }
        }
    }    
}

















// EventQueueTest.c --------------------------------------------------------------------------------

#ifdef TEST

#include <string.h>

void HelloWorld()
{
    printf("Hello World.\n");
}

void OneShot()
{
    printf("One shot.\n");
}

void Periodic()
{
    static uint32_t    i = 0;
    printf("Periodic(%d).\n",i);
    i++;
}

int main()
{
    memset(&timedEventHandlers[0], 0x00, sizeof(timedEventHandlers));

    Call( HelloWorld );
    CallAfter_ms( OneShot, 1000 );
    CallEvery_ms( Periodic, 500 );

    while(true)
    {
        CheckTimedEventHandlers();
        EventLoop();
    }
}

#endif





