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
    printf("\n\n");

    uint8_t     element[100]    = {0};
    strcpy(&element[0], "one");
    PersistentCircularBufferAdd( &context, &element[0] );
#if 0
    //PersistentCircularBufferAdd( &context, "two" );
    //PersistentCircularBufferAdd( &context, "three" );

    ShowState( &context );

    PersistentCircularBufferRemove( &context, &element[0] );
    printf("[%s]\n", element);
#endif    
}



