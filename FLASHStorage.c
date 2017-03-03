
#include <stdint.h>
#include <stdio.h>


#define MAX_YEARS           (15)
#define WAKEUP_CYCLES       (MAX_YEARS*365*48)
#define MAX_ERASE_CYCLES    (50000)
#define MAX_REDUNDNACY      (WAKEUP_CYCLES/MAX_ERASE_CYCLES)

#define PAGE_SIZE           (4096)

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))




//
// Empty struct for padding out to a full page size..
//
typedef struct
{
    uint8_t     data[PAGE_SIZE];

}  __attribute__((packed)) Page;



//
//
//
typedef union
{
    struct
    {
        uint32_t        pageCRC;

        uint32_t        one;
        uint32_t        two;
        uint32_t        three;
        uint8_t         four[8];
        uint16_t        five;
        uint32_t        six;
        uint16_t        seven;        

    } Data;

    Page    raw;
    
}  __attribute__((packed)) PageOne ;


//
//
//
typedef union
{
    struct
    {
        uint32_t        pageCRC;

        uint32_t        one;
        uint32_t        two;
        uint32_t        three;
        uint8_t         four[8];
        uint16_t        five;
        uint32_t        six;
        uint16_t        seven;        

    } Data;

    Page    raw;
    
}  __attribute__((packed)) PageTwo ;


//
//
//
typedef struct
{
    PageOne     one[MAX_REDUNDNACY];

    PageTwo     two[MAX_REDUNDNACY];

}  __attribute__((packed)) BlockDeviceLayout;




void main()
{
    BUILD_BUG_ON( sizeof(PageOne) != PAGE_SIZE);
    BUILD_BUG_ON( (sizeof(BlockDeviceLayout)%PAGE_SIZE) != 0);

    printf("%d %d %d\n", (int)sizeof(PageOne), (int)sizeof(PageTwo), (int)sizeof(BlockDeviceLayout) );
}


