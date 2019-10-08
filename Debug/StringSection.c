
#include <stdio.h>
#include <stdarg.h>



//
// This will be stored in .xxx_section
//
char* specialString __attribute__ ((__section__(".blaa"))) = "Hello World!";

//
// This will be stored into .data
//
char* normalString  = "Hello Hello Hello";


void myLoggingFn(const char* format, ...)
{
    va_list args;
    va_start(args, format);
 
    printf("[%p]\n", format);
 
    va_end(args);
}

#define LOG_DEBUG(format, ... ) {static const char t[] __attribute__ ((__section__(".logdebug"))) =format;myLoggingFn( t, ## __VA_ARGS__ );}

int main()
{
    printf("[%s]",normalString);
    printf("[%s]",specialString);

    LOG_DEBUG("One two three %d", 1);
    LOG_DEBUG("four five six %d", 2);
    LOG_DEBUG("seven eight nine");
}


