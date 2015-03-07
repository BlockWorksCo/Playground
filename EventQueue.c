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

typedef void (*Handler)();

void PANIC();


DECLARE_QUEUE( HandlerQueue, Handler, 4);



void PANIC()
{
    printf("PANIC!\n");
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








void HelloWorld()
{
    printf("Hello World.\n");
}

int main()
{
    Call( HelloWorld );

    while(true)
    {
        EventLoop();
    }
}








