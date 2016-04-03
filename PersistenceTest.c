//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//






#include "PersistentCircularBuffer.h"
#include <stdio.h>
#include <string.h>





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
    uint8_t     element[100]    = {0};
    uint32_t*   value           = (uint32_t*)&element[0];

    PersistentCircularBufferRemove( &context, &element[0] );
    printf("[%d]\n", *value);
}


int main()
{
    FLASHDeviceInitialise();
    PersistentCircularBufferInitialise( &context, &layout );
    //ShowState( &context );


    uint32_t    i = 0;
    while(i<1000)
    {
        Add();
        Remove();

        i++;
    }

    PersistentCircularBufferFlush( &context );
}



