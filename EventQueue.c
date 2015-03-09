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
    while(true);
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


struct
{
    Handler     handler;
    uint32_t    interval;
    uint32_t    firingTime;

} timedEventHandlers[8] = {0};


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

struct
{
    Handler     handler;
    bool*       blockingFlag;

} blockedEventHandlers[8];


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


// ResourceSharingTest.c --------------------------------------------------------------------------------


typedef struct
{
    uint8_t*    dataIn;
    uint8_t*    dataOut;
    uint8_t     cs;
    uint8_t     numberOfBytesToTransfer;
    Handler     completionHandler;

} TransferRequest;

DECLARE_QUEUE( TransferRequestQueue, TransferRequest, 16);

TransferRequest     currentRequest;
TransferRequest     nullRequest = 
{
    .dataIn                  = 0, 
    .dataOut                 = 0, 
    .numberOfBytesToTransfer = 0, 
    .completionHandler       = 0
};

void TransferFinished()
{
    printf("<TransferFinished>\n");
    Call( currentRequest.completionHandler );
}

void PerformTransfer( uint8_t* dataIn, uint8_t* dataOut, uint8_t numberOfBytesToTransfer, Handler completionHandler )
{
    TransferRequest     request     = 
    {
        .dataIn                  = dataIn, 
        .dataOut                 = dataOut, 
        .numberOfBytesToTransfer = numberOfBytesToTransfer, 
        .completionHandler       = completionHandler
    };

    TransferRequestQueuePut( request );

    //
    // Simulate a real transfer, will be called via ISR in reality.
    //
    currentRequest  = TransferRequestQueueGet( nullRequest );
    CallAfter_ms( TransferFinished, 100 );
}

// EventQueueTest.c --------------------------------------------------------------------------------

#ifdef TEST

void HelloWorld()
{
    printf("Hello World.\n");
}

void OneShot()
{
    printf("One shot.\n");
}

void TransferComplete()
{
    printf("TransferComplete\n");
}

void Tock()
{
    printf("Tock.\n");

    static uint8_t  in[8];
    static uint8_t  out[8];

    PerformTransfer( &in[0], &out[0], 8, TransferComplete );
}

void Tick()
{
    static uint32_t    i = 0;
    printf("Tick(%d).\n",i);
    i++;

    CallAfter_ms( Tock, 250 );
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
    Call( HelloWorld );
    CallAfter_ms( OneShot, 1000 );
    CallEvery_ms( Tick, 500 );

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





