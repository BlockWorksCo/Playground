





#include "PersistentCircularBuffer.h"
#include <stdio.h>





PersistentCircularBufferLayout  layout  = LAYOUT(0, 16, 100);
PersistentCircularBufferContext context = {0};



int main()
{
    FLASHDeviceInitialise();
    PersistentCircularBufferInitialise( &context, &layout );

    PersistentCircularBufferAdd( &context, "one" );
    PersistentCircularBufferAdd( &context, "two" );
    PersistentCircularBufferAdd( &context, "three" );

    ShowState( &context );

    uint8_t     element[100];
    PersistentCircularBufferRemove( &context, &element[0] );
    printf("[%s]\n", element);
}



