


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


//
//
//
class ByteStream
{

public:

    ByteStream(uint8_t* _data, uint32_t _numberOfBytes) :
               data( _data ),
               numberOfBytes( _numberOfBytes ),
               currentPosition( 0 )
    {
    }


    bool Get_UINT8( uint8_t& value )
    {
        if(currentPosition < numberOfBytes)
        {
            value   = data[currentPosition];
            currentPosition++;
            return true;
        }
        else
        {
            return false;
        }
    }

private:

    uint8_t*    data;
    uint32_t    numberOfBytes;
    uint32_t    currentPosition;

};





//
//
//
class HDLCFrame
{

public:

    HDLCFrame( ByteStream& _stream ) :
        stream( _stream )
    {
    }


    bool Get_UINT8( uint8_t& value )
    {
        bool            success     = true;
        static bool     withinFrame = false;
        
        //
        // Make sure we're within a frame.
        //
        if(withinFrame == false)
        {
            do
            {
                success = stream.Get_UINT8(value);
                if(value == HDLC_FRAME_DELIMIT)
                {
                    //
                    // Frame start detected, use next byte.
                    //
                    withinFrame     = true;
                }

            } while( withinFrame == false && success == true );
        }

        //
        // Get the byte from within the frame.
        //
        if(success == true)
        {
            success = stream.Get_UINT8(value);
            if(value == HDLC_FRAME_DELIMIT)
            {
                //
                // Frame end detected.
                //
                withinFrame     = false;
                success         = false;
            }
        }

        return success;
    }

    

private:

    static const uint8_t    HDLC_FRAME_DELIMIT     = 0x7E;

    ByteStream&  stream;

};






//
//
//
int main()
{
    printf("DLMS encode/decoder test\n");

    {
        uint8_t     readClockRequest[] = {0x7E,0xA0,0x19,0x95,0x75,0x54,0x68,0x35,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x08,0x00,0x00,0x01,0x00,0x00,0xFF,0x01,0x00,0x0D,0xFD,0x7E};
        ByteStream  requestStream( &readClockRequest[0], sizeof(readClockRequest) );
        HDLCFrame   requestFrame( requestStream );

        bool        success             = false;
        uint8_t     value               = 0;

        do
        {
            success = requestFrame.Get_UINT8(value);
            if(success == true)
            {
                printf("%02x ", value);
            }

        } while( success == true );
    }

    printf("\n");
}






