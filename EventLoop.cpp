





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










sigjmp_buf  env;
int         val;
void INThandler(int sig);


int fg_put_message_and_wait_for_response()
{
    /*
    // Put message into q....
    */
    /*fg_put_message( app_proxy_qid, (uint16)NULL, (void *)ind_msg );*/
   
    /*
    // Save the current context for later return.
    // *Note, we only use sigsetjmp/siglongjmp instead of setjmp/longjmp purely because
    // we are simulating interrutps via signals.
    */
    static int  data    = 0;
    val     = sigsetjmp(env, 1);    
    if( val == 0 )
    {
        /*
        // Do other scheduler-type stuff.... (need to stop this context if not busy, we cant return here)
        */
        while(1)
        {
            /*printf("Tick...\n");*/
            sleep(1);
        }
    }
    else
    {
        /*
        // Event has been received, return now....
        */
        data    = data + 1;
    }

    return data;
}









void AppProcessEvent()
{
    if (val == 0)
    {
        siglongjmp(env, 1);
    }
}

void App()
{
    /*
    // Traditional sequential, single context code.
    */
    printf ("Data: %d\n", fg_put_message_and_wait_for_response() );
    printf ("Data: %d\n", fg_put_message_and_wait_for_response() );
    printf ("Data: %d\n", fg_put_message_and_wait_for_response() );
}

void Blaa()
{
    static int count = 0;
    printf("\nBlaa(%d)!\n", count);
}












/*
// Substitute ISR... just something asynchronous to the main context....
*/
void INThandler(int sig)
{
    AppProcessEvent();
}









main()
{
    signal(SIGINT, INThandler);

    QueueEventHandler(Blaa);
    QueueEventHandler(App);
    QueueEventHandler(Blaa);
    while(true)
    {
        ProcessEvents();
        Sleep();
    }

    return 0;
}



