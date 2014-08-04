





#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>










#define NUMBER_OF_ELEMENTS(a)   (sizeof(a)/sizeof(a[0]))
#define PANIC(code)             {printf("PANIC!(%d",code);while(true);}

typedef void (*EventHandler)();

EventHandler    eventQ[8];
int             eventQHead  = 0;
int             eventQTail  = 0;

void ProcessEvents()
{
    while( eventQHead != eventQTail )
    {
        EventHandler     currentHandler  = eventQ[eventQTail];        
        eventQTail  = (eventQTail + 1) % NUMBER_OF_ELEMENTS(eventQ);
        currentHandler();
    }
}

void QueueEventHandler(EventHandler handler)
{
    int     nextHead    = (eventQHead + 1) % NUMBER_OF_ELEMENTS(eventQ);

    if( nextHead != eventQTail )
    {
        eventQ[eventQHead]  = handler;
        eventQHead          = nextHead;
    }
    else
    {
        PANIC(1);
    }
}

void Sleep()
{
    /*
    // Put CPU into low-power state, wait for an interrupt to wake.
    */
}









void One()
{
    static int count = 0;
    printf("\nOne(%d)!\n", count);
}

void Two()
{
    static int count = 0;
    printf("\nTwo(%d)!\n", count);
}

void Three()
{
    static int count = 0;
    printf("\nThree(%d)!\n", count);
}














main()
{
    QueueEventHandler(One);
    QueueEventHandler(Two);
    QueueEventHandler(Three);
    while(true)
    {
        ProcessEvents();
        Sleep();
    }

    return 0;
}



