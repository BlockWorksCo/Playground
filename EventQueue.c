//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//


#include <stdio.h>
#include "Queue.h"
#include <stdint.h>
#include <stdbool.h>
#include "Utilities.h"
#include <time.h>

typedef void (*Handler)();

typedef struct
{
    Handler     handler;
    uint32_t    interval;
    uint32_t    firingTime;

} TimedEventHandler;



void PANIC();


DECLARE_QUEUE( HandlerQueue, Handler, 16);
TimedEventHandler   timedEventHandlers[8];


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



void Call( Handler handler )
{
    HandlerQueuePut( handler );
}


void EventLoop()
{
    Handler     handler;

    do
    {
        handler = HandlerQueueGet( NULL );
        if( handler != NULL )
        {
            handler();
        }

    } while( handler != NULL );
}




void CallEvery_ms( Handler handler, uint32_t interval )
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(timedEventHandlers); i++)
    {
        if( timedEventHandlers[i].handler == NULL )
        {
            timedEventHandlers[i].handler       = handler;
            timedEventHandlers[i].interval      = interval;
            timedEventHandlers[i].firingTime    = CurrentTimestamp() + interval;
        }

        break;
    }
}

void CallAfter_ms( Handler handler, uint32_t interval )
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(timedEventHandlers); i++)
    {
        if( timedEventHandlers[i].handler == NULL )
        {
            timedEventHandlers[i].handler       = handler;
            timedEventHandlers[i].interval      = 0;
            timedEventHandlers[i].firingTime    = CurrentTimestamp() + interval;
        }

        break;
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
                    timedEventHandlers[i].handler   = NULL;
                }
                else
                {
                    timedEventHandlers[i].firingTime    = timestamp + timedEventHandlers[i].interval;                    
                }
            }
        }
    }    
}




#ifdef TEST

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





