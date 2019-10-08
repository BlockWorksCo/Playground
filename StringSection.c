
#include <stdio.h>



//
// This will be stored in .xxx_section
//
char* specialString __attribute__ ((__section__(".xxx_section"))) = "Hello World!";

//
// This will be stored into .data
//
char* normalString  = "Hello Hello Hello";


void myLoggingFn(const char* format, ...)
{
}

#define LOG_DEBUG(format, ... ) {static const char* t __attribute__ ((__section__(".xxx_section"))) =format;myLoggingFn( t, ## __VA_ARGS__ );}

int main()
{
    printf("[%s]",normalString);
    printf("[%s]",specialString);

    LOG_DEBUG("One two three %d", 1);
    LOG_DEBUG("four five six %d", 2);
    LOG_DEBUG("seven eight nine");
}


