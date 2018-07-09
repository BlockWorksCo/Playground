
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "message.pb.h"





void printHex(uint8_t value)                                                                    
{                                                                                               
    printf("%02X",value);                                                                       
}                                                                                               
                                                                                                
void printHexData(uint8_t* data, uint32_t numberOfBytes)                                        
{                                                                                               
    for(uint32_t i=0; i<numberOfBytes; i++)                                                     
    {                                                                                           
        printHex(data[i]);                                                                      
    }                                                                                           
}                                                                                               
                                                                                                


bool decodeList(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    uint64_t value;
    if (!pb_decode_varint(stream, &value))
    {
        printf("[done]\n");
        return false;
    }
    else
    {
        printf("[%ld]\n", value);
        return false;
    }
}


bool encodeList(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    for(uint32_t i=0; i<5; i++)
    {
        if (!pb_encode_tag_for_field(stream, field))
        {
            return false;
        }
        pb_encode_varint(stream, 9+i);
        printf("encoding %d\n",9+i);
    }
    
    return true;
}

// 08 FA 01 10 AF 01 18 09 18 0A 18 0B 18 0C 18 0D
int main(int argc, char **argv)
{
    {
        AMessage    message = AMessage_init_zero;

        message.a   = 0xfa;
        message.b   = 0xaf;
        message.has_b    = true;
        message.list.funcs.encode   = &encodeList;

        size_t  bufferSize  = 0;
        pb_get_encoded_size( &bufferSize, AMessage_fields, &message );
        printf("encoded size = %zu\n", bufferSize);
        uint8_t*    buffer  = malloc(bufferSize);
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferSize );

        /* Now encode it and check if we succeeded. */
        if (pb_encode(&stream, AMessage_fields, &message))
        {
            printf("\n");
            printHexData( buffer, bufferSize );
            printf("\n");
        
            printf("---decoding---\n");
            pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));
            AMessage    message2    = AMessage_init_zero;
            message2.list.funcs.decode  = &decodeList;
            pb_decode( &stream, AMessage_fields, &message2 );

            printf("a=%d\n",message2.a);
            printf("b=%d\n",message2.b);
            return 0; /* Success */
        }
        else
        {
            fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
            return 1; /* Failure */
        }
    }
}
