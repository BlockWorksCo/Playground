





#include "PersistentCircularBuffer.h"






PersistentCircularBufferLayout  layout  = LAYOUT(0, 16, 100);
PersistentCircularBufferContext context = {0};



int main()
{
    FLASHDeviceInitialise();
    PersistentCircularBufferInitialise( &context, &layout );

    PersistentCircularBufferAdd( &context, "one" );
    PersistentCircularBufferAdd( &context, "two" );
    PersistentCircularBufferAdd( &context, "three" );
}



