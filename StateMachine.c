
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


// definition of fsm data.
typedef struct
{
    uint32_t    count;
} FSMAStateData;

// instantiation of fsm data.
FSMAStateData   fsmaStateData =
{
    .count  = 0,
};

// enumeration of the different states for fsm.
typedef enum
{
    TickState,
    TockState,
    EndState,

} FSMAState;

// state machine.
bool fsmaProcessEvent( StateMachine* fsm )
{
    FSMAStateData*  data    = (FSMAStateData*)fsm->data; 
    bool            finishedFlag    = false;

    switch( fsm->state )
    {
        case TickState:
            printf("tick\n");
            fsm->state  = TockState;
            break;

        case TockState:
            printf("tock (%d) \n",data->count);
            data->count++;
            if(data->count >= 10)
            {
                fsm->state  = EndState;
            }
            else
            {
                fsm->state  = TickState;
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

// instantiation of state machine.
StateMachine    fsma   =
{
    .state          = TickState,
    .ProcessEvent   = fsmaProcessEvent,
    .data           = &fsmaStateData,
};


/////////////////////////////////////////////////////////////////////////////////////////



// return next FSM in queue.
StateMachine* UnqueueFSM()
{
    return &fsma;
}

void QueueFSM( StateMachine* fsm )
{
}


// entry point.
int main()
{
    // forever...
    while(true)
    {
        // Choose the next FSM...
        StateMachine*   fsm = UnqueueFSM();

        if( fsm != NULL )
        {
            // ...and run it until finished.
            bool    finishedFlag    = false;
            do
            {
                fsm->ProcessEvent( fsm );

            } while( finishedFlag == false );
        }
    }
}



