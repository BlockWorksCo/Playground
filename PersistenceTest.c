//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//






#include "PersistentCircularBuffer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




PersistentCircularBufferLayout  layout  = LAYOUT(0, 16, 100);
PersistentCircularBufferContext context = {0};



void Add()
{
    static uint8_t      element[100]    = {0};
    uint32_t*           value           = (uint32_t*)&element[0];

    (*value)++;
    PersistentCircularBufferAdd( &context, &element[0] );
}

void Remove()
{
    static uint32_t checkValue      = 0;
    uint8_t         element[100]    = {0};
    uint32_t*       value           = (uint32_t*)&element[0];

    PersistentCircularBufferRemoveFirst( &context, &element[0] );

    checkValue++;
    if(checkValue == *value)
    {
        printf("[%d]\n", *value);
    }
    else
    {
        printf("BAD! [%d != %d]\n", checkValue, *value);
        exit(-1);
    }
}


int main()
{
    FLASHDeviceInitialise();
    PersistentCircularBufferInitialise( &context, &layout );
    ShowState( &context );

    srand(3);
    uint32_t    i = 0;
    while(i<1000000)
    {
        static uint32_t     outstanding     = 0;
        uint32_t    capacity                = PersistentCircularBufferCapacity(&context);
        uint32_t    numberOfElements        = PersistentCircularBufferNumberOfElements(&context);

        if(outstanding != numberOfElements)
        {
            printf("MISCOUNT\n");
            exit(-1);
        }

        if( ((rand()%100) > 50) && ((capacity - numberOfElements) > 0) )
        {
            Add();
            outstanding++;
        }
        else if(numberOfElements > 0)
        {
            Remove();     
            outstanding--;           
        }

        i++;
    }

    PersistentCircularBufferFlush( &context );
}



