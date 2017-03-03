


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


typedef struct 
{
    uint32_t    baseAddress;
    uint32_t    size;
    bool        subRegions[8];

} Window;


Window  windows[3];


uint8_t     map[4096];


//
//
//
void RenderWindow( uint8_t* map, Window* window )
{
    uint32_t    start           = window->baseAddress & (~(window->size-1));
    uint32_t    end             = start + window->size;
    uint32_t    subRegionSize   = window->size / 8;

    for(uint32_t i=0; i<8; i++)
    {
        uint8_t     value     = '-';
        if(window->subRegions[i] == true)
        {
            value   = '*';
        }

        memset(&map[i*subRegionSize], value, subRegionSize);
    }

}


//
//
//
void RenderWindows( Window* window )
{
    memset( &map[0], ' ', sizeof(map) );

    RenderWindow( &map[0], &window[0] );
    RenderWindow( &map[0], &window[1] );
    RenderWindow( &map[0], &window[2] );

    for(uint32_t i=0; i<sizeof(map); i+=8)
    {
        printf("%c", map[i] );
    }
    printf("\n");
}


//
//
//
void AllocateWindows( uint32_t position, Window windows[3] )
{
    uint32_t    breakPoint;

    windows[0].size     = 1024;
    windows[1].size     = windows[0].size / 8;
    windows[2].size     = windows[1].size / 8;

    windows[0].baseAddress  = 0;
    breakPoint  = (position-windows[0].baseAddress) / (windows[0].size / 8);
    memset( &windows[0].subRegions[0], true, 8 );
    for(uint32_t i=0; i<breakPoint; i++)
    {
        windows[0].subRegions[i]    = false;
    }
    printf("b0 = %d + (%d * %d)\n", windows[0].baseAddress, breakPoint, windows[0].size/8 );

    windows[1].baseAddress  = windows[0].baseAddress + (breakPoint * (windows[0].size / 8));
    breakPoint  = (position-windows[1].baseAddress) / (windows[1].size / 8);
    memset( &windows[1].subRegions[0], true, 8 );
    for(uint32_t i=0; i<breakPoint; i++)
    {
        windows[1].subRegions[i]    = false;
    }
    printf("b0 = %d + (%d * %d)\n", windows[1].baseAddress, breakPoint, windows[1].size/8 );

    windows[2].baseAddress  = windows[1].baseAddress + (breakPoint * (windows[1].size / 8));
    breakPoint  = (position-windows[2].baseAddress) / (windows[2].size / 8);
    memset( &windows[2].subRegions[0], true, 8 );
    for(uint32_t i=0; i<breakPoint; i++)
    {
        windows[2].subRegions[i]    = false;
    }
    printf("b0 = %d + (%d * %d)\n", windows[2].baseAddress, breakPoint, windows[2].size/8 );




    for(uint32_t i=0; i<8; i++)
    {
        printf("(%d)\n", windows[2].subRegions[i] );
    }
}


//
//
//
void main()
{
    AllocateWindows( 103, &windows[0] );

    RenderWindows( &windows[0] );
}




