
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "message.pb.h"
#include "Calendar.pb.h"





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





bool decodeSeasonProfile(pb_istream_t *stream, const pb_field_t *field, void **arg)                          
{                                                                                                  
    SeasonProfile msg = SeasonProfile_init_zero;                                                       

    pb_decode(stream, SeasonProfile_fields, &msg);                                                   

    printf("seasonId=%d month=%d dayOfMonth=%d weekId=%d\n\n",msg.identifier, msg.month, 
                                                msg.dayOfMonth, msg.weekProfileId);
                                                                                                   
    return true;                                                                                   
}                                                                                                  


bool decodeWeekProfile(pb_istream_t *stream, const pb_field_t *field, void **arg)                          
{                                                                                                  
    WeekProfile msg = WeekProfile_init_zero;                                                       

    pb_decode(stream, WeekProfile_fields, &msg);                                                   

    printf("m=%d t=%d w=%d t=%d f=%d s=%d s=%d\n",msg.mondayId, msg.tuesdayId, msg.wednesdayId,
                                        msg.thursdayId, msg.fridayId, msg.saturdayId, msg.sundayId);
    printf("weekId=%d\n\n",msg.identifier);
                                                                                                   
    return true;                                                                                   
}                                                                                                  




bool decodeDaySchedule(pb_istream_t *stream, const pb_field_t *field, void **arg)                          
{                                                                                                  
    TariffDaySchedule msg = TariffDaySchedule_init_zero;                                                       
                                                                                                   
    pb_decode(stream, TariffDaySchedule_fields, &msg);                                                   

    printf("hours=%d minutes=%d seconds=%d tariffId=%d\n",msg.hours, msg.minutes, msg.seconds, msg.tariffId);
                                                                                                   
    return true;                                                                                   
}                                                                                                  

bool decodeDayProfile(pb_istream_t *stream, const pb_field_t *field, void **arg)                          
{                                                                                                  
    DayProfile msg = DayProfile_init_zero;                                                       

    msg.daySchedule.funcs.decode  = &decodeDaySchedule;                                                  
    msg.daySchedule.arg  = NULL;                                                                      

    pb_decode(stream, DayProfile_fields, &msg);                                                   

    printf("dayId=%d\n\n",msg.identifier);
                                                                                                   
    return true;                                                                                   
}                                                                                                  

bool decodeRateCalendar(pb_istream_t *stream, const pb_field_t *field, void **arg)                          
{                                                                                                  
    RateCalendar msg = RateCalendar_init_zero;                                                       

    msg.seasonProfiles.funcs.decode  = &decodeSeasonProfile;
    msg.seasonProfiles.arg  = NULL;                                                                      

    msg.weekProfiles.funcs.decode  = &decodeWeekProfile;
    msg.weekProfiles.arg  = NULL;                                                                      

    msg.dayProfiles.funcs.decode  = &decodeDayProfile;
    msg.dayProfiles.arg  = NULL;                                                                      

    pb_decode(stream, RateCalendar_fields, &msg);                                                   
                                                                                                   
    return true;                                                                                   
}                                                                                                  


                                                                                                

bool encodeWeekProfiles(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    {
        WeekProfile msg = 
        {
            .identifier     = 1,
            .mondayId       = 1,
            .tuesdayId      = 2,
            .wednesdayId    = 3,
            .thursdayId     = 4,
            .fridayId       = 5,
            .saturdayId     = 6,
            .sundayId       = 7,
        };

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, WeekProfile_fields, &msg);
    }

    {
        WeekProfile msg = 
        {
            .identifier     = 1,
            .mondayId       = 2,
            .tuesdayId      = 2,
            .wednesdayId    = 2,
            .thursdayId     = 2,
            .fridayId       = 2,
            .saturdayId     = 2,
            .sundayId       = 2,
        };

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, WeekProfile_fields, &msg);
    }

    return true;
}


bool encodeSeasonProfiles(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    {
        SeasonProfile msg = 
        {
            .identifier     = 1,
            .month          = 3,
            .dayOfMonth     = 4,
            .weekProfileId  = 5,
        };

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, SeasonProfile_fields, &msg);
    }

    {
        SeasonProfile msg = 
        {
            .identifier     = 1,
            .month          = 5,
            .dayOfMonth     = 6,
            .weekProfileId  = 7,
        };

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, SeasonProfile_fields, &msg);
    }

    return true;
}


bool encodeDaySchedule(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    {
        TariffDaySchedule msg = {.hours=1, .minutes=2, .seconds=3, .tariffId=4};

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, TariffDaySchedule_fields, &msg);
    }

    {
        TariffDaySchedule msg = {.hours=2, .minutes=2, .seconds=3, .tariffId=4};

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, TariffDaySchedule_fields, &msg);
    }

    {
        TariffDaySchedule msg = {.hours=3, .minutes=2, .seconds=3, .tariffId=4};

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, TariffDaySchedule_fields, &msg);
    }

    return true;
}

bool encodeDayProfiles(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    {
        DayProfile msg = {.identifier=1};
        msg.daySchedule.funcs.encode    = &encodeDaySchedule;

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, DayProfile_fields, &msg);
    }

    {
        DayProfile msg = {.identifier=2};
        msg.daySchedule.funcs.encode    = &encodeDaySchedule;

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, DayProfile_fields, &msg);
    }

    {
        DayProfile msg = {.identifier=3};
        msg.daySchedule.funcs.encode    = &encodeDaySchedule;

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, DayProfile_fields, &msg);
    }

    return true;
}

bool encodeRateCalendar(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    {
        RateCalendar msg = {0};

        msg.dayProfiles.funcs.encode    = &encodeDayProfiles;
        msg.dayProfiles.arg             = NULL;

        msg.weekProfiles.funcs.encode   = &encodeWeekProfiles;
        msg.weekProfiles.arg            = NULL;

        msg.seasonProfiles.funcs.encode = &encodeSeasonProfiles;
        msg.seasonProfiles.arg          = NULL;

        pb_encode_tag_for_field(stream, field);
        pb_encode_submessage(stream, RateCalendar_fields, &msg);
    }

    return true;
}




int main(int argc, char **argv)
{
    {
        //
        // Create a message.
        //
        MeterConfigMessage    message = MeterConfigMessage_init_zero;

        message.rateCalendar.funcs.encode   = &encodeRateCalendar;

        //
        // Get the size of the encoded message.
        //
        size_t  bufferSize  = 0;
        pb_get_encoded_size( &bufferSize, MeterConfigMessage_fields, &message );
        printf("encoded size = %zu\n", bufferSize);

        //
        // Allocate a buffer to hold the encoded message.
        //
        uint8_t*    buffer  = malloc(bufferSize);
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferSize );

        //
        // Encode the message into the buffer.
        //
        if (pb_encode(&stream, MeterConfigMessage_fields, &message))
        {
            //
            // Show the pb data.
            //
            printf("\n");
            printHexData( buffer, bufferSize );
            printf("\n");
        
            //
            // Decode the message from the buffer.
            //
            pb_istream_t stream = pb_istream_from_buffer(buffer, bufferSize);
            MeterConfigMessage    decoded    = MeterConfigMessage_init_zero;
            decoded.rateCalendar.funcs.decode  = &decodeRateCalendar;
            pb_decode( &stream, MeterConfigMessage_fields, &decoded );

            return 0;
        }
        else
        {
            fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
            return 1; /* Failure */
        }
    }
}
