
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


//
//
//
typedef struct
{
    uint32_t    base;
    uint32_t    size;

} Window;


//
//
//
typedef struct
{
    Window  windows[8];
    uint8_t stack[1024];

} Domain;

Domain  currentDomain;


//
//
//
Domain  domains[4]  =
{
    { 
        .windows=   {
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0}}, 
        .stack={0} 
    },
    { 
        .windows=   {
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0}}, 
        .stack={0} 
    },
    { 
        .windows=   {
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0}}, 
        .stack={0} 
    },
    { 
        .windows=   {
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0},
                        {.base=0, .size=0}}, 
        .stack={0} 
    },
};


//
// Search for the domain associated with the specified function. PANIC if not found.
//
Domain* DomainForFunction( uint32_t fnAddress )
{
    return &domains[0];
}


//
//
//
void EnterDomain( Domain* savedDomain )
{

    //
    // Save the current domain on the callers stack.
    //
    memcpy( &savedDomain, &currentDomain, sizeof(savedDomain) );

    //
    //
    //
    uint32_t    callerAddress   = 0;

    //
    //
    //
    Domain*     newDomain   = DomainForFunction( callerAddress );
    memcpy( &currentDomain, newDomain, sizeof(currentDomain) );

    //
    // Swap the stacks.
    //
}


//
//
//
void ExitDomain( Domain* savedDomain )
{
    //
    // Restore the saved domain.
    //
    memcpy( &currentDomain, savedDomain, sizeof(currentDomain) );

    //
    // Restore the stack.
    //
}


void Hello()
{
    Domain      savedDomain;
    EnterDomain( &savedDomain );

    printf("Hello World\n");

    ExitDomain( &savedDomain );
}


//
//
//
int main(int argc, char const *argv[])
{
    Hello();
    return 0;
}




