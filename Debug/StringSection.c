
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include <stddef.h>



//
// This will be stored into .data and so wont be removed.
//
char* normalString  = "Hello Hello Hello";


#define LOG_DEBUG(format, ... ) {static const char t[] __attribute__ ((__section__(".logdebug"))) =format;myLoggingFn( t, ## __VA_ARGS__ );}

void myLoggingFn(const char* format, ...)
{
    va_list args;
    va_start(args, format);
 
    extern const void * const logdebug_start;
    uintptr_t   logdebugBase  = (uintptr_t)&logdebug_start;

    printf("format(%td),\n", (ptrdiff_t)(format-logdebugBase));

    while (*format != '\0') {
        if (*format == 'd') {
            int i = va_arg(args, int);
            printf("int(%d),", i);
        } else if (*format == 'c') {
            int c = va_arg(args, int);
            printf("char(%c),", c);
        } else if (*format == 'f') {
            double d = va_arg(args, double);
            printf("float(%f)\n", d);
        }
        ++format;
    }
 
    va_end(args);
}


int main()
{
    printf("[%s]",normalString);

    LOG_DEBUG("One two three %d", 1);
    LOG_DEBUG("four five six %d", 2);
    LOG_DEBUG("seven eight nine");
}


