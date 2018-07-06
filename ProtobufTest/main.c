/* Attempts to test all the datatypes supported by ProtoBuf.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pb_encode.h>
#include "alltypes.pb.h"

int main(int argc, char **argv)
{
    AllTypes alltypes = AllTypes_init_zero;
    
        alltypes.req_int32         = -1001;
        alltypes.req_int64         = -1002;
        alltypes.req_uint32        = 1003;
        alltypes.req_uint64        = 1004;
        alltypes.req_sint32        = -1005;
        alltypes.req_sint64        = -1006;
        alltypes.req_bool          = true;
        
        alltypes.req_fixed32       = 1008;
        alltypes.req_sfixed32      = -1009;
        alltypes.req_float         = 1010.0f;
        
        alltypes.req_fixed64       = 1011;
        alltypes.req_sfixed64      = -1012;
        alltypes.req_double        = 1013.0;
        
        strcpy(alltypes.req_string, "1014");
        alltypes.req_bytes.size = 4;
        memcpy(alltypes.req_bytes.bytes, "1015", 4);
        strcpy(alltypes.req_submsg.substuff1, "1016");
        alltypes.req_submsg.substuff2 = 1016;
        alltypes.req_enum = MyEnum_Truth;
        memcpy(alltypes.req_fbytes, "1019", 4);
        
        alltypes.rep_int32_count = 5; alltypes.rep_int32[4] = -2001;
        alltypes.rep_int64_count = 5; alltypes.rep_int64[4] = -2002;
        alltypes.rep_uint32_count = 5; alltypes.rep_uint32[4] = 2003;
        alltypes.rep_uint64_count = 5; alltypes.rep_uint64[4] = 2004;
        alltypes.rep_sint32_count = 5; alltypes.rep_sint32[4] = -2005;
        alltypes.rep_sint64_count = 5; alltypes.rep_sint64[4] = -2006;
        alltypes.rep_bool_count = 5; alltypes.rep_bool[4] = true;
        
        alltypes.rep_fixed32_count = 5; alltypes.rep_fixed32[4] = 2008;
        alltypes.rep_sfixed32_count = 5; alltypes.rep_sfixed32[4] = -2009;
        alltypes.rep_float_count = 5; alltypes.rep_float[4] = 2010.0f;
        
        alltypes.rep_fixed64_count = 5; alltypes.rep_fixed64[4] = 2011;
        alltypes.rep_sfixed64_count = 5; alltypes.rep_sfixed64[4] = -2012;
        alltypes.rep_double_count = 5; alltypes.rep_double[4] = 2013.0;
        
        alltypes.rep_string_count = 5; strcpy(alltypes.rep_string[4], "2014");
        alltypes.rep_bytes_count = 5; alltypes.rep_bytes[4].size = 4;
        memcpy(alltypes.rep_bytes[4].bytes, "2015", 4);

        alltypes.rep_submsg_count = 5;
        strcpy(alltypes.rep_submsg[4].substuff1, "2016");
        alltypes.rep_submsg[4].substuff2 = 2016;
        alltypes.rep_submsg[4].has_substuff3 = true;
        alltypes.rep_submsg[4].substuff3 = 2016;
        
        alltypes.rep_enum_count = 5; alltypes.rep_enum[4] = MyEnum_Truth;
        alltypes.rep_emptymsg_count = 5;
        
        alltypes.rep_fbytes_count = 5;
        memcpy(alltypes.rep_fbytes[4], "2019", 4);
        
        alltypes.req_limits.int32_min  = INT32_MIN;
        alltypes.req_limits.int32_max  = INT32_MAX;
        alltypes.req_limits.uint32_min = 0;
        alltypes.req_limits.uint32_max = UINT32_MAX;
        alltypes.req_limits.int64_min  = INT64_MIN;
        alltypes.req_limits.int64_max  = INT64_MAX;
        alltypes.req_limits.uint64_min = 0;
        alltypes.req_limits.uint64_max = UINT64_MAX;
        alltypes.req_limits.enum_min   = HugeEnum_Negative;
        alltypes.req_limits.enum_max   = HugeEnum_Positive;
    
    alltypes.end = 1099;
    
    {
        uint8_t buffer[AllTypes_size];
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        
        /* Now encode it and check if we succeeded. */
        if (pb_encode(&stream, AllTypes_fields, &alltypes))
        {
            SET_BINARY_MODE(stdout);
            fwrite(buffer, 1, stream.bytes_written, stdout);
            return 0; /* Success */
        }
        else
        {
            fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
            return 1; /* Failure */
        }
    }
}
