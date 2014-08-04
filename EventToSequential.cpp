





#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>













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

    App();

    return 0;
}



