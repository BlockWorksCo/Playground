


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>


typedef enum
{
    TypeInt8        = 0,
    TypeInt16,
    TypeInt24,
    TypeInt32,
    TypeInt64,
    TypeText,
    TypePointer,

} ValueType;



void OutputInt32( uint32_t value )
{
    printf("Int32[%08x]\n",value);
} 

void OutputInt8( uint8_t value )
{
    printf("Int8[%02x]\n",value);
} 

void lprintf(const char* format, ...)
{
    va_list ap;

    va_start(ap, format);

    for(uint32_t i=0; i<strlen(format); i++ ) {

        if( format[i] == '%' ) {

            //
            uint8_t     typeCode    = format[i+1];
            i++;

            //
            switch(typeCode) {
                case 'c':
                {
                    uint32_t    value   = va_arg(ap, uint32_t);
                    OutputInt8( value );
                    break;
                }

                case 'd':
                case 'u':
                case 'x':
                {
                    uint32_t    value   = va_arg(ap, uint32_t);
                    OutputInt32( value );
                    break;
                }

                default:
                    break;
            }

        }
    }

    va_end(ap);
}




int main()
{
    lprintf("  [%d]   -%c-   %d",(uint8_t)1,2,(uint8_t)3);
}


