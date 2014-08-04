/*
// The EventLoop has the following features:
// - Allows multiple concurrent state-machines to be written without any OS support.
// IOTW concurrency on bare-metal, without an OS.
// - Allows very tight control over sleeping behaviour. on return from a handler, the system
// will be able to sleep. no need to wait for a context switch to an idle-task as in an RTOS.
// - Stack usage of a typical many-concurrent-state-machines type app is reduced compared to
// having one stack per-thread & one thread per state-machine. Instead, we simply have one 
// stack, reducing RAM usage.
// - Simplicity. The code is very clear and deterministic.
// - Can be extended to include priorities, parameters & timing to the events. Scales up nicely from
// this minimal mechanism to a full featired scheduler.
// - Optional mechanism similar to EventToSequential can make a more traditional sequential
// program-flow from individual events.
// - Handler-execution is non-deterministic. real-time aspects are handled in the ISR which then
// performs minimal-necessary processing before queueing an event to continue the work. This
// is the typicical top&bottom-half ISR style.
// - Compared to a typical super-loop style, the EventLoop produces a more loosly-coupled 
// architecture. Each state-machine knows nothing about the others, just the EventLoop library.
// - Composable. Its a library, not a framework, meaning it doesn't impose any unnecessary 
// structure on your code.
// - High level, there is nothing but plain ANSI C99 here. no low-level context saving or stack
// fiddling (as compared to an RTOS).
// - Trivially portable to *every* architecture.
*/
#include <stdint.h>

#define NUMBER_OF_ELEMENTS(a)   (sizeof(a)/sizeof(a[0]))
#define PANIC(code)             {/*printf("PANIC!(%d",code);*/while(true);}

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





/*
// Example set of event handlers....
*/
#include <stdio.h>

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






/*
// Main routine, showing main event-sleep-loop
*/
main()
{
    QueueEventHandler(One);
    QueueEventHandler(Two);
    QueueEventHandler(Three);

    while(true)
    {
        /*
        // The main loop process all current events and then returns directly here to perform
        // an immediate sleep. No context switch to an idle task prior to sleep here...
        */
        ProcessEvents();
        Sleep();
    }

    return 0;
}



