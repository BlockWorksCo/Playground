


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>


typedef enum
{
    TypePrintfFormat,       // The format of the log-entry.
    TypeVersionHash,        // To identify image on host in a way that the source can generate easily.
    TypeTimeBase,           // log-entry timestamp is timebase+linenumber.
    TypeMachineID,          // Because source-ip may be dynamic.

} ValueType;





/*------------------------------------------------------------------------*/


void OutputPrintfFormat( uint8_t** packet, size_t* packetSpaceAvailable, const void* value )
{
    extern uint32_t rodata_start;
    ptrdiff_t offsetFromRODataOrigin = value - (void*)&rodata_start;

    printf("PrintfFormat[%td]\n",offsetFromRODataOrigin);

    *packet[0]   = TypePrintfFormat;
    *packet  += 1;
    *packetSpaceAvailable   -= 1;
    memcpy( *packet, &offsetFromRODataOrigin, sizeof(offsetFromRODataOrigin) );
    *packet  += sizeof(offsetFromRODataOrigin);
    *packetSpaceAvailable   -= sizeof(offsetFromRODataOrigin);
} 

void OutputInt32( uint8_t** packet, size_t* packetSpaceAvailable, uint32_t value )
{
    printf("Int32[%08x]\n",value);

    memcpy( *packet, &value, sizeof(value) );
    *packet  += sizeof(value);
    *packetSpaceAvailable   -= sizeof(value);
} 

void OutputInt8( uint8_t** packet, size_t* packetSpaceAvailable, uint8_t value )
{
    printf("Int8[%02x]\n",value);

    memcpy( *packet, &value, sizeof(value) );
    *packet  += sizeof(value);
    *packetSpaceAvailable   -= sizeof(value);
} 

void OutputText( uint8_t** packet, size_t* packetSpaceAvailable, char* value )
{
    printf("Text[%zd,%s]\n",strlen(value),value);

    *packet[0]   = strlen(value);
    *packet  += 1;
    *packetSpaceAvailable   -= 1;
    memcpy( *packet, value, strlen(value) );
    *packet  += strlen(value);
    *packetSpaceAvailable   -= sizeof(value);
} 

void lprintf( uint8_t** packet, size_t* packetSpaceAvailable, const char* format, ... )
{
    va_list ap;

    va_start(ap, format);

    OutputPrintfFormat( packet,packetSpaceAvailable, format );

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
                    OutputInt8( packet,packetSpaceAvailable, value );
                    break;
                }

                case 'd':
                case 'u':
                case 'x':
                {
                    uint32_t    value   = va_arg(ap, uint32_t);
                    OutputInt32( packet,packetSpaceAvailable, value );
                    break;
                }

                case 's':
                {
                    char*   value   = va_arg(ap, char*);
                    OutputText( packet,packetSpaceAvailable, value );
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




void rprintf( uint8_t** packet, size_t packetSize, char* format )
{
    printf( "PrintfFormat[%s]\n", format );

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
                    //*packet += 1;
                    uint8_t    value   = 0;
                    memcpy( &value, *packet, sizeof(value) );
                    *packet += sizeof(value);
                    printf("Int8[%08x]\n",value);
                    break;
                }

                case 'd':
                case 'u':
                case 'x':
                {
                    //*packet += 1;
                    uint32_t    value   = 0;
                    memcpy( &value, *packet, sizeof(value) );
                    *packet += sizeof(value);
                    printf("Int32[%08x]\n",value);
                    break;
                }

                case 's':
                {
                    //*packet += 1;
                    uint8_t numberOfBytes   = 0;
                    memcpy( &numberOfBytes, *packet, sizeof(numberOfBytes) );
                    *packet += sizeof(numberOfBytes);

                    char    value[256]   = {0};
                    memcpy( &value, *packet, numberOfBytes );
                    *packet += numberOfBytes;
                    printf("Text[%s]\n",value);
                };

                default:
                    break;
            }

        }
    }

}

void consumePacket( uint8_t** packet, size_t packetSize )
{
    while( packetSize > 0 ) {

        ValueType type    = (ValueType) *packet[0];
        *packet += 1;
        packetSize--;

        switch( type ) {

            case TypeTimeBase:
                // store as current timebase.
                break;

            case TypeVersionHash:
                // load appropriate image file with matching version-hash.
                break;

            case TypeMachineID:
                // Output machineID to support multiple sources. Maybe use sourceIP instead?
                break;

            case TypePrintfFormat:
            {
                // printf operation.
                extern uint32_t rodata_start;
                ptrdiff_t   value   = 0;
                memcpy( &value, *packet, sizeof(value) );
                *packet += sizeof(value);
                char*   format  = ((char*)&rodata_start) + value;
                rprintf( packet, packetSize, format );
                break;
            }

            default:
                break;
        }

        break;
    }
}


/*------------------------------------------------------------------------*/

int main()
{
    //
    //
    //
    uint8_t     outputPacket[1024]  = {0};
    uint8_t*    packet              = &outputPacket[0];
    size_t      packetSpaceAvailable= sizeof(outputPacket);

    char    temp1[16]   = "Hello World";
    lprintf( &packet,&packetSpaceAvailable, "  [%d]   -%01c-   %d %s",(uint8_t)1,2,(uint8_t)3,temp1);

    //
    //
    //
    printf("\n");
    ptrdiff_t   numberOfBytesInPacket   = packet - &outputPacket[0];
    for(uint32_t i=0; i<numberOfBytesInPacket; i++) {
        printf("%02x ",outputPacket[i]);
    }
    printf("\n");

    //
    //
    //
    printf("\n");
    printf("%ld bytes in packet.\n",numberOfBytesInPacket);

    packet  = &outputPacket[0];
    consumePacket( &packet, numberOfBytesInPacket );
}


