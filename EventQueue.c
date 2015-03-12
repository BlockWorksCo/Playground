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


void DispatchHandlers()
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


// PredicatedEvents.c --------------------------------------------------------------------------------



#include "Utilities.h"

struct
{
    Handler     handler;
    bool        (*predicate)();

} predicatedEventHandlers[8];


void CallWhenPredicateIsTrue( Handler handler, bool (*predicate)() )
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(predicatedEventHandlers); i++)
    {
        if( predicatedEventHandlers[i].handler == 0 )
        {
            predicatedEventHandlers[i].handler      = handler;
            predicatedEventHandlers[i].predicate    = predicate;
            break;
        }

    }    

}


void CheckPredicatedEventHandlers()
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(predicatedEventHandlers); i++)
    {
        if( predicatedEventHandlers[i].handler != 0 )
        {
            if( predicatedEventHandlers[i].predicate() == true )
            {
                Call( predicatedEventHandlers[i].handler );

                predicatedEventHandlers[i].handler   = 0;
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

void TransferFinished();

void PollRequestQueue()
{
    currentRequest  = TransferRequestQueueGet( nullRequest );
    if( currentRequest.numberOfBytesToTransfer != 0 )
    {
        //
        // Simulate a real transfer, will be called via ISR in reality.
        //
        CallAfter_ms( TransferFinished, 100 );        
    }    
}

void TransferFinished()
{
    printf("<TransferFinished>\n");
    Call( currentRequest.completionHandler );

    PollRequestQueue();
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

    PollRequestQueue();
}

// PersistentStorage.c --------------------------------------------------------------------------------

#include <string.h>

#define BLOCK_SIZE          (1024)
#define NUMBER_OF_BLOCKS    (128)


uint8_t     simulatedFLASH[BLOCK_SIZE*NUMBER_OF_BLOCKS];

void WriteBlock( uint32_t address, uint8_t* data, uint32_t numberOfBytes, Handler completionHandler )
{
    memcpy( &simulatedFLASH[address], data, numberOfBytes );

    Call( completionHandler );
}

void ReadBlock( uint32_t address, uint8_t* data, uint32_t numberOfBytes, Handler completionHandler )
{
    memcpy( data, &simulatedFLASH[address], numberOfBytes );    
    
    Call( completionHandler );
}

void EraseBlock( uint32_t address, Handler completionHandler )
{
    uint32_t    blockNumber     = address / BLOCK_SIZE;
    memset( &simulatedFLASH[blockNumber*BLOCK_SIZE], 0xff, BLOCK_SIZE );
    
    Call( completionHandler );
}

void EraseDevice( Handler completionHandler )
{
    memset( &simulatedFLASH[0], 0xff, sizeof(simulatedFLASH) );
    
    Call( completionHandler );
}


// FileStorage.c --------------------------------------------------------------------------------


uint32_t fileMap[]  =
{
    0,
    1,
    10,
    30,
    40,
};

void WriteFileBlock( uint8_t fileNumber, uint32_t address, uint8_t* data, uint32_t numberOfBytes, Handler completionHandler )
{
    WriteBlock( (fileMap[fileNumber]*BLOCK_SIZE)+address, data, numberOfBytes, completionHandler );
}

void ReadFileBlock( uint8_t fileNumber, uint32_t address, uint8_t* data, uint32_t numberOfBytes, Handler completionHandler )
{
    ReadBlock( (fileMap[fileNumber]*BLOCK_SIZE)+address, data, numberOfBytes, completionHandler );
}

void EraseFileBlock( uint8_t fileNumber, uint32_t address, Handler completionHandler )
{
    EraseBlock( (fileMap[fileNumber]*BLOCK_SIZE)+address, completionHandler );
}


// LogStorage.c --------------------------------------------------------------------------------

#define LOG_ENTRY_SIZE      (128)
#define LOG_FILE_NUMBER     (1)

uint32_t    lastEntry   = 0xffffffff;


//
// FindLastEntry state machine.
//
uint32_t    FindLastEntryReadAddress       = 0;
Handler     FindLastEntryCompletionEvent   = 0;
uint8_t     FindLastEntryLogEntry          = {0};

void FindLastEntryReadComplete()
{
    FindLastEntryReadAddress    += LOG_ENTRY_SIZE;
    ReadFileBlock( LOG_FILE_NUMBER, FindLastEntryReadAddress, &FindLastEntryLogEntry, LOG_ENTRY_SIZE, FindLastEntryReadComplete );
}

uint32_t FindLastEntry( Handler completionHandler )
{
    FindLastEntryCompletionEvent    = completionHandler;
    if(lastEntry == 0xffffffff)
    {
        FindLastEntryReadAddress    = 0;
        ReadFileBlock( LOG_FILE_NUMBER, FindLastEntryReadAddress, &FindLastEntryLogEntry, LOG_ENTRY_SIZE, FindLastEntryReadComplete );
    }
    else
    {
        Call( completionHandler );
    }

    return lastEntry;
}


//
// WriteLog state machine.
//
uint8_t*    WriteLogLogEntry            = 0;
Handler     WriteLogCompletionHandler   = 0;

void WriteLogLastEntryAvailable()
{
    uint32_t    address     = lastEntry;
    WriteFileBlock( LOG_FILE_NUMBER, address, WriteLogLogEntry, LOG_ENTRY_SIZE, WriteLogCompletionHandler );
}

void WriteLog( uint8_t* data, Handler completionHandler )
{
    WriteLogLogEntry    = data;
    WriteLogCompletionHandler   = completionHandler;
    FindLastEntry( WriteLogLastEntryAvailable );
}


//
// ReadLog state machine.
//
uint8_t*    ReadLogLogEntry             = 0;
Handler     ReadLogCompletionHandler    = 0;

void ReadLogLastEntryAvailable()
{
    uint32_t    address     = lastEntry+1;
    ReadFileBlock( LOG_FILE_NUMBER, address, ReadLogLogEntry, LOG_ENTRY_SIZE, ReadLogCompletionHandler );
}

void ReadLog( uint8_t* data, Handler completionHandler )
{
    ReadLogLogEntry             = data;
    ReadLogCompletionHandler    = completionHandler;
    FindLastEntry( ReadLogLastEntryAvailable );
}


// KeyValueStore.c --------------------------------------------------------------------------------

#include <stddef.h>

#define MAX_KEY_VALUE   (128)

#define DECLARE_KEY_VALUE_STORAGE( type, name )  type name##Storage
#define DECLARE_KEY( name )  name = offsetof( KeyValueData, name##Storage )
#define SET_KEY_VALUE( key, value )    keyValueData.key##Storage = value;KeyValueStoreWritten(key)
#define GET_KEY_VALUE( key )    keyValueData.key##Storage 

typedef struct
{
    DECLARE_KEY_VALUE_STORAGE( uint32_t, Value1 );
    DECLARE_KEY_VALUE_STORAGE( uint32_t, Value2 );
    DECLARE_KEY_VALUE_STORAGE( uint32_t, Value3 );
    DECLARE_KEY_VALUE_STORAGE( uint32_t, Value4 );
    DECLARE_KEY_VALUE_STORAGE( uint32_t, Value5 );

} KeyValueData;
KeyValueData keyValueData;

typedef enum
{
    DECLARE_KEY( Value1 ),
    DECLARE_KEY( Value2 ),
    DECLARE_KEY( Value3 ),
    DECLARE_KEY( Value4 ),
    DECLARE_KEY( Value5 ),

    MaxKeyValue = sizeof(KeyValueData)

} Key;

void KeyValueStoreBackedUp()
{

}

void KeyValueStoreErased()
{
    WriteFileBlock( 1, 0, (uint8_t*)&keyValueData, sizeof(keyValueData), KeyValueStoreBackedUp );
}

void KeyValueStoreWritten( uint32_t key )
{
    //
    // Write to persistent storage.
    //
    EraseFileBlock( 1, 0, KeyValueStoreErased );
}


// EventQueueTest.c --------------------------------------------------------------------------------

#ifdef TEST

uint32_t    seconds     = 0;

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
    seconds++;
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



bool TenSecondsHavePassed()
{
    if( seconds > 10 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BigTick()
{
    printf("*** big tick ***\n");
}

int main()
{
    Call( HelloWorld );
    CallAfter_ms( OneShot, 1000 );
    CallEvery_ms( Tick, 500 );

    CallAfter_ms( PullTheTrigger, 5000 );
    CallWhenUnblocked( Bang, &trigger );

    CallWhenPredicateIsTrue( BigTick, TenSecondsHavePassed );

    SET_KEY_VALUE( Value1, 123 );
    uint32_t temp = GET_KEY_VALUE( Value1 );

    while(true)
    {
        CheckTimedEventHandlers();
        CheckBlockedEventHandlers();
        CheckPredicatedEventHandlers();
        DispatchHandlers();
    }
}

#endif





