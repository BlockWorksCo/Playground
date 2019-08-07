


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
    TypePrintfFormat,

} ValueType;





/*------------------------------------------------------------------------*/


void OutputPrintfFormat( uint8_t** packet, size_t packetSize, const void* value )
{
    extern uint32_t rodata_start;
    ptrdiff_t offsetFromRODataOrigin = value - (void*)&rodata_start;

    printf("PrintfFormat[%td]\n",offsetFromRODataOrigin);

    *packet[0]   = TypePrintfFormat;
    *packet  += 1;
    memcpy( *packet, &offsetFromRODataOrigin, sizeof(offsetFromRODataOrigin) );
    *packet  += sizeof(offsetFromRODataOrigin);
} 

void OutputInt32( uint8_t** packet, size_t packetSize, uint32_t value )
{
    printf("Int32[%08x]\n",value);

    *packet[0]   = TypeInt32;
    *packet  += 1;
    memcpy( *packet, &value, sizeof(value) );
    *packet  += sizeof(value);
} 

void OutputInt8( uint8_t** packet, size_t packetSize, uint8_t value )
{
    printf("Int8[%02x]\n",value);

    *packet[0]   = TypeInt8;
    *packet  += 1;
    memcpy( *packet, &value, sizeof(value) );
    *packet  += sizeof(value);
} 

void OutputText( uint8_t** packet, size_t packetSize, char* value )
{
    printf("Text[%zd,%s]\n",strlen(value),value);

    *packet[0]   = TypeText;
    *packet  += 1;
    *packet[0]   = strlen(value);
    *packet  += 1;
    memcpy( *packet, value, strlen(value) );
    *packet  += strlen(value);
} 

void lprintf( uint8_t** packet, size_t packetSize, const char* format, ... )
{
    va_list ap;

    va_start(ap, format);

    OutputPrintfFormat( packet,packetSize, format );

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
                    OutputInt8( packet,packetSize, value );
                    break;
                }

                case 'd':
                case 'u':
                case 'x':
                {
                    uint32_t    value   = va_arg(ap, uint32_t);
                    OutputInt32( packet,packetSize, value );
                    break;
                }

                case 's':
                {
                    char*   value   = va_arg(ap, char*);
                    OutputText( packet,packetSize, value );
                };

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

/*------------------------------------------------------------------------*/





/*------------------------------------------------------------------------*/

int main()
{
    //
    //
    //
    uint8_t     outputPacket[1024]  = {0};
    uint8_t*    packet              = &outputPacket[0];

    char    temp1[16]   = "Hello World";
    lprintf( &packet,sizeof(outputPacket), "  [%d]   -%01c-   %d %s",(uint8_t)1,2,(uint8_t)3,temp1);

    //
    //
    //
    ptrdiff_t   numberOfBytesInPacket   = packet - &outputPacket[0];
    printf("\n");
    printf("%ld bytes in packet.\n",numberOfBytesInPacket);
}


