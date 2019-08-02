


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>


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



void OutputPointer( const void* value )
{
    extern uint32_t rodata_start;
    ptrdiff_t offsetFromRODataOrigin = value - (void*)&rodata_start;
    printf("Base[%p]\n",&rodata_start);
    printf("Ptr[%td]\n",offsetFromRODataOrigin);
} 

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

    OutputPointer( format );

    for(uint32_t i=0; i<strlen(format); i++ ) {

        if( format[i] == '%' ) {

            //
            uint8_t     typeCode    = format[i+1];
            uint32_t    length      = 0;
            i++;

            // length modifier.
            if( typeCode == '0' ) {
                typeCode = format[i+2];
                length   = format[i+1] - '0';    
                //printf("length is %d\n",length);
                i   += 2;
            }

            // types.
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


void OutputSkinnyLogHeader()
{
    // output 32-bit version-hash (for image-identification on remote).

    // output 32-bit machine-id-hash (for source-identification on remote).

    // output 32-bit time-base for this packet.
}



int main()
{
    lprintf("  [%d]   -%01c-   %d",(uint8_t)1,2,(uint8_t)3);
}


