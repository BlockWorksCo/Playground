
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include <stddef.h>



//
// This will be stored into .data and so wont be removed.
//
char* normalString  = "Hello Hello Hello";


#define LOG_DEBUG(format, ... ) {static const char t[] __attribute__ ((__section__(".logdebug"))) =format;myLoggingFn( t, ## __VA_ARGS__ );}

uint32_t paramTypeListForStringId( uint32_t stringId )
{
    return 0x00abcdef;
}

//
// Supports the following types:
// 0 = list-terminator.
// 1 = %d = int32_t
// 2 = %u = uint32_t
// 3 = %s = null-terminated-string.
// 4 = 
// 5 = 
// 6 = 
// 7 = 
//
void myLoggingFn(const char* format, ...)
{
    va_list args;
    va_start(args, format);
 
    extern const void * const logdebug_start;
    uintptr_t   logdebugBase  = (uintptr_t)&logdebug_start;

    ptrdiff_t   stringOffset    = (ptrdiff_t)format-logdebugBase;
    uint32_t    stringId        = (uint32_t)stringOffset;
    printf("format(%"PRIu32"),", stringId );

    uint16_t    paramTypeList   = paramTypeListForStringId(stringId);

    uint8_t     paramIndex      = 0;
    uint8_t     paramType       = 0;
    do {
        paramType   = paramTypeList & 0x3;
        printf("<%d>",paramType);
        paramTypeList   >>= 3; 
        paramIndex++;
    } while( (paramType != 0) && (paramIndex<8) );

            //int i = va_arg(args, int);
            //int c = va_arg(args, int);
            //double d = va_arg(args, double);
 
    va_end(args);
}


int main()
{
    printf("[%s]\n\n",normalString);

    LOG_DEBUG("One two three %d", 1);
    LOG_DEBUG("four five six %d this is a much longer string", 2);
    LOG_DEBUG("seven eight nine");
}


