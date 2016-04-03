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



int main()
{
    FLASHDeviceInitialise();
    PersistentCircularBufferInitialise( &context, &layout );
    ShowState( &context );

    uint8_t     element[100]    = {0};
    strcpy(&element[0], "one");
    PersistentCircularBufferAdd( &context, &element[0] );
    ShowState( &context );
#if 1
    strcpy(&element[0], "two");
    PersistentCircularBufferAdd( &context, &element[0] );

    strcpy(&element[0], "three");
    PersistentCircularBufferAdd( &context, &element[0] );

    ShowState( &context );
#endif
    memset(&element[0], 0, sizeof(element));
    PersistentCircularBufferRemove( &context, &element[0] );
    printf("[%s]\n", element);
    ShowState( &context );
#if 1
    memset(&element[0], 0, sizeof(element));
    PersistentCircularBufferRemove( &context, &element[0] );
    printf("[%s]\n", element);

    memset(&element[0], 0, sizeof(element));
    PersistentCircularBufferRemove( &context, &element[0] );
    printf("[%s]\n", element);
#endif
    PersistentCircularBufferFlush( &context );
}



