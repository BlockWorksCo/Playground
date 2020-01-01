//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//



#include "Common.h"
#include "Queue.h"
#include "EventQueue.h"
#include "ErrorHandling.h"

DECLARE_QUEUE( HandlerQueue,            Handler, 16);
DECLARE_QUEUE( InterruptHandlerQueue,   Handler, 16);


void Call( Handler handler )
{
    HandlerQueuePut( handler );
}


void CallFromInterrupt( Handler handler )
{
    InterruptHandlerQueuePut( handler );
}


void DispatchHandlers()
{
    Handler     handler;
    bool        noEventsLeftToProcess;

    do
    {
        noEventsLeftToProcess   = true;

        handler = HandlerQueueGet( 0 );
        if( handler != 0 )
        {
            handler();
            noEventsLeftToProcess   = false;
        }

        handler = InterruptHandlerQueueGet( 0 );
        if( handler != 0 )
        {
            handler();
            noEventsLeftToProcess   = false;
        }

    } while( noEventsLeftToProcess == false );
}



