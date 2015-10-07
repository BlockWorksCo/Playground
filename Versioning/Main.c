




#include <stdio.h>
#include <stddef.h>
#include "DataModel.h"
#include "BlockDevice.h"
#include "DataModelUpgrade.h"
#include "DataModelMap.h"
#include "CircularBuffer.h"




//
//
//
int main()
{
    BlockDeviceInitialise();


    {
        //
        // Blank slate. This is a cold boot situation.
        //
        DataModelInitialise();

        uint32_t    one     = 0;
        uint32_t    two     = 0;
        uint32_t    three   = 0;
        uint32_t    four    = 0;
        DataModelGetItemOne( &one );
        DataModelGetItemTwo( &two );
        DataModelGetItemThree( &three );
        DataModelGetItemFour( &four );

        printf("1) %d\n", one);
        printf("2) %d\n", two);
        printf("3) %d\n", three);
        printf("4) %d\n", four);        
    }



    {
        //
        // Force an old image into the BlockDevice.
        //
        DataModelMap_v0     map_v0  = 
        {
            .version    = 0x00,
            .one        = 43,
            .two        = 123,
            .three      = 321,
            .four       = 567,
        };

        BlockDeviceWrite(0, sizeof(DataModelMap_v0), (uint8_t*)&map_v0 );

        //
        // Now initialise as if we had just reset.
        //
        DataModelInitialise();

        uint32_t    one     = 0;
        uint32_t    two     = 0;
        uint32_t    three   = 0;
        uint32_t    four    = 0;
        DataModelGetItemOne( &one );
        DataModelGetItemTwo( &two );
        DataModelGetItemThree( &three );
        DataModelGetItemFour( &four );

        printf("1) %d\n", one);
        printf("2) %d\n", two);
        printf("3) %d\n", three);
        printf("4) %d\n", four);        
    }



    {
        //
        // A new image exists now, so we shouldn't upgrade in this simulated reset.
        //
        DataModelInitialise();

        uint32_t    one     = 0;
        uint32_t    two     = 0;
        uint32_t    three   = 0;
        uint32_t    four    = 0;
        DataModelGetItemOne( &one );
        DataModelGetItemTwo( &two );
        DataModelGetItemThree( &three );
        DataModelGetItemFour( &four );

        printf("1) %d\n", one);
        printf("2) %d\n", two);
        printf("3) %d\n", three);
        printf("4) %d\n", four);        
    }


    {
        CircularBufferContext   context     = {0};
        CircularBufferContextInitialise( &context, offsetof(DataModelMap, queueA), sizeoffield(DataModelMap, queueA), 12);

        printf("- %d\n",context.offset);
        printf("- %d\n",context.elementSize);
        printf("- %d\n",context.numberOfElements);

        CircularBufferPut( &context, "Zero       ");
        /*
        CircularBufferPut( &context, "One        ");
        CircularBufferPut( &context, "Two        ");
        CircularBufferPut( &context, "Three      ");
        CircularBufferPut( &context, "Four       ");
        CircularBufferPut( &context, "Five       ");
        CircularBufferPut( &context, "Six        ");
        CircularBufferPut( &context, "Seven      ");
        CircularBufferPut( &context, "Eight      ");
        CircularBufferPut( &context, "Nine       ");
        CircularBufferPut( &context, "Ten        ");
        CircularBufferPut( &context, "Eleven     ");
        CircularBufferPut( &context, "Twelve     ");
        */

        uint8_t     temp[16];
        CircularBufferGet( &context, &temp[0] );
        printf("(%s)\n", temp);
    }
}



