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


typedef struct
{
    char        text[32];
    uint32_t    timestamp;

} LogEntry;

void PANIC();


DECLARE_QUEUE( ByteQueue, uint8_t, 4);
DECLARE_QUEUE( LogEntryQueue, LogEntry, 4);



void PANIC()
{
    printf("PANIC!\n");
}

//
//
//
void main()
{
    ByteQueuePut( 1 );
    ByteQueuePut( 2 );
    ByteQueuePut( 3 );

    printf("%02x\n", ByteQueueGet(0) );
    printf("%02x\n", ByteQueueGet(0) );
    printf("%02x\n", ByteQueueGet(0) );


    LogEntry    entryA  = {"One", 0x00000001};
    LogEntryQueuePut( entryA );

    LogEntry    entryB  = {"Two", 0x00000002};
    LogEntryQueuePut( entryB );

    LogEntry    entryNULL  = {"<None>", 0x00000000};
    LogEntry    entry;

    entry   = LogEntryQueueGet( entryNULL );
    printf("%08x, %s\n", entry.timestamp, entry.text );

    entry   = LogEntryQueueGet( entryNULL );
    printf("%08x, %s\n", entry.timestamp, entry.text );
}



