


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>





void lprintf(const char* format, ...)
{
    va_list ap;

    va_start(ap, format);

    for(uint32_t i=0; i<strlen(format); i++ ) {
        if( format[i] == '%' ) {
            uint32_t    value   = va_arg(ap, uint32_t);
            printf("value = %08x\n", value);
        }
    }


    va_end(ap);
}




int main()
{
    lprintf("  [%d]   -%d-   %d",(uint8_t)1,2,(uint8_t)3);
}


