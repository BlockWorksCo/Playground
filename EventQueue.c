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

uint32_t CurrentTimestamp_ms()
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








// TimedEvents.c --------------------------------------------------------------------------------


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
            timedEventHandlers[i].firingTime    = CurrentTimestamp_ms() + interval;
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
            timedEventHandlers[i].firingTime    = CurrentTimestamp_ms() + interval;
            break;
        }

    }    
}


void CheckTimedEventHandlers()
{
    uint32_t    timestamp   = CurrentTimestamp_ms();

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











// BlockedEvents.c --------------------------------------------------------------------------------



#include "Utilities.h"

typedef struct
{
    Handler     handler;
    bool*       blockingFlag;

} BlockedEventHandler;

BlockedEventHandler   blockedEventHandlers[8];


void CallWhenUnblocked( Handler handler, bool* blockingFlag )
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(blockedEventHandlers); i++)
    {
        if( blockedEventHandlers[i].handler == 0 )
        {
            blockedEventHandlers[i].handler         = handler;
            blockedEventHandlers[i].blockingFlag    = blockingFlag;
            break;
        }

    }    

}


void CheckBlockedEventHandlers()
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(blockedEventHandlers); i++)
    {
        if( blockedEventHandlers[i].handler != 0 )
        {
            if( *blockedEventHandlers[i].blockingFlag == true )
            {
                Call( blockedEventHandlers[i].handler );

                blockedEventHandlers[i].handler   = 0;
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

bool trigger    = false;

void PullTheTrigger()
{
    trigger     = true;
    printf("pulled the trigger...\n");
}

void Bang()
{
    printf("Bang!\n");
}

int main()
{
    memset(&timedEventHandlers[0], 0x00, sizeof(timedEventHandlers));

    Call( HelloWorld );
    CallAfter_ms( OneShot, 1000 );
    CallEvery_ms( Periodic, 500 );

    CallAfter_ms( PullTheTrigger, 5000 );
    CallWhenUnblocked( Bang, &trigger );

    while(true)
    {
        CheckTimedEventHandlers();
        CheckBlockedEventHandlers();
        EventLoop();
    }
}

#endif





