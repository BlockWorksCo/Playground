
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
                                                                                                

bool decodeSchedule(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    printf("decodeSchedule\n");
    
    Schedule    msg = Schedule_init_zero;
    pb_decode(stream, Schedule_fields, &msg);
    
    printf("\tstart=%d script=%d\n",msg.start,msg.scriptId);
    return true;
}

bool decodeSub(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    ASubMessage msg = ASubMessage_init_zero;

    printf("decodeSub\n");
    msg.schedule.funcs.decode  = &decodeSchedule;
    msg.schedule.arg  = NULL;
    pb_decode(stream, ASubMessage_fields, &msg);
    printf("   one=%d two=%d\n",msg.one,msg.two);

    return true;
}

bool decodeList(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    uint64_t value  = 0;
    pb_decode_varint(stream, &value);
    printf("[%ld]\n", value);

    return true;
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
    }
    
    return true;
}


bool encodeSchedules(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    {
        Schedule msg = {.start=999,.scriptId=876};
        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, Schedule_fields, &msg);
    }

    {
        Schedule msg = {.start=55,.scriptId=66};
        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, Schedule_fields, &msg);
    }

    {
        Schedule msg = {.start=77,.scriptId=88};
        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, Schedule_fields, &msg);
    }

    return true;
}

bool encodeSub(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    {
        ASubMessage msg = {.one=111,.two=222};
        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, ASubMessage_fields, &msg);
    }

    {
        ASubMessage msg = {.one=333,.two=444};
        msg.schedule.funcs.encode   = &encodeSchedules;
        msg.schedule.arg            = NULL;
        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, ASubMessage_fields, &msg);
    }

    {
        ASubMessage msg = {.one=555,.two=666};
        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, ASubMessage_fields, &msg);
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
        message.sub.funcs.encode     = &encodeSub;

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
            pb_istream_t stream = pb_istream_from_buffer(buffer, bufferSize);
            AMessage    message2    = AMessage_init_zero;
            message2.list.funcs.decode  = &decodeList;
            message2.sub.funcs.decode  = &decodeSub;
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
