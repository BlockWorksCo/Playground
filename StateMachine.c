
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


/////////////////////////////////////////////////////////////////////////////////////////



typedef struct
{
    uint32_t    state;
    bool        (*ProcessEvent)();
    void*       data;

} StateMachine;



/////////////////////////////////////////////////////////////////////////////////////////


typedef struct
{
    uint32_t    count;
} FSMAStateData;

FSMAStateData   fsmaStateData =
{
    .count  = 0,
};

bool fsmaProcessEvent( StateMachine* fsm )
{
    FSMAStateData*  data    = (FSMAStateData*)fsm->data; 
    bool            finishedFlag    = false;

    switch( fsm->state )
    {
        case 0:
            printf("tick\n");
            fsm->state  = 1;
            break;

        case 1:
            printf("tock (%d) \n",data->count);
            data->count++;
            if(data->count >= 10)
            {
                fsm->state  = 2;
            }
            else
            {
                fsm->state  = 0;
            }
            break;

        case 2:
            finishedFlag    = true;
            break;

        default:
            break;
    }
    
    return finishedFlag;
}


StateMachine    fsma   =
{
    .state          = 0,
    .ProcessEvent   = fsmaProcessEvent,
    .data           = &fsmaStateData,
};


/////////////////////////////////////////////////////////////////////////////////////////




StateMachine* nextFSM()
{
    return &fsma;
}


int main()
{
    while(true)
    {
        // Choose the next FSM.
        StateMachine*   fsm = nextFSM();

        if( fsm != NULL )
        {
            // Run the next FSM until finished.
            bool    finishedFlag    = false;
            do
            {
                fsm->ProcessEvent( fsm );

            } while( finishedFlag == false );
        }
    }
}



