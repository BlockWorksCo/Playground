
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "message.pb.h"

int main(int argc, char **argv)
{
    {
        AMessage    message = AMessage_init_zero;

        message.a   = 33;
        message.b   = 44;
        message.has_b    = true;

        uint8_t buffer[AMessage_size];
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        
        /* Now encode it and check if we succeeded. */
        if (pb_encode(&stream, AMessage_fields, &message))
        {
            pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));
            AMessage    message2    = AMessage_init_zero;
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
