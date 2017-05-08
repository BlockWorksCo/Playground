


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
typedef struct
{
    uint8_t     d0;
    uint8_t     d1;
    uint8_t     d2;
    uint8_t     d3;
    uint8_t     d4;
    uint8_t     d5;

} LogicalName;


//
//
//
class DLMSParser
{

public:

    DLMSParser()
    {
    }


    bool ProcessLLC(uint32_t value)
    {
        //
        // Destination_LSAP: 0xE6;
        // Source_LSAP: 0xE6 for command frames and 0xE7 for response frames;
        // LLC_Quality: it is reserbed for future use and its value must always be 0x00.
        //
        uint32_t    commandResponseBit   = value & (1<<8);
        if( commandResponseBit == 0)
        {
            printf("LLC command: %08x\n", value);
        }
        else
        {
            printf("LLC response: %08x\n", value);
        }
    }

    bool ProcessRequestType(uint8_t tag, uint8_t length)
    {
        printf("RequestType: tag %02x, length %02x\n", tag, length);
        switch(tag)
        {
            case 0x06:
                printf("\tWRITE Request\n");
                break;
            
            case 0x0d:
                printf("\tWRITE Response\n");
                break;
            
            case 0x05:
                printf("\tREAD Request\n");
                break;
            
            case 0x0c:
                printf("\tREAD Response\n");
                break;
            
            case 0xc0:
                printf("\tGET Request\n");
                break;
            
            case 0xc4:
                printf("\tGET Response\n");
                break;
            
            case 0xc1:
                printf("\tSET Request\n");
                break;
            
            case 0xc5:
                printf("\tSET Response\n");
                break;
            
            case 0x93:
                printf("\tSNRM Request\n");
                break;
            
            case 0x73:
                printf("\tUA Response\n");
                break;
            
            case 0x60:
                printf("\tAARQ Request\n");
                break;
            
            case 0x61:
                printf("\tAARE Response\n");
                break;
            
            default:
                break;
        }
    }

    bool ProcessInvokeId(uint8_t value)
    {
        printf("InvokeID: %02x\n", value);
    }

    bool ProcessInterfaceClass(uint16_t value)
    {
        printf("InterfaceClass: %04x\n", value);
    }

    bool ProcessLogicalName(LogicalName value)
    {
        printf("InterfaceClass: %02x.%02x.%02x.%02x.%02x.%02x\n", value.d0, value.d1, value.d2, value.d3, value.d4, value.d5);
    }

    bool ProcessAttributeNumber(uint16_t value)
    {
        printf("AttributeNumber: %04x\n", value);
    }



    //
    // Called if the check-sequence passes.
    //
    void Abort()
    {
        printf("Abort\n");
    }

    void Commit()
    {
        printf("Complete\n");
    }



    bool Parse( uint8_t value )
    {
        bool                success     = false;
        static uint32_t     position    = 0;        
        static uint8_t      requestTag;
        static uint8_t      requestLength;
        static LogicalName  logicalName;
        static uint8_t      invokeId;
        static uint8_t      classId[2];
        static uint8_t      attributeNumber[2];
        static uint8_t      llc[4];


        //
        // Parse the header bytes.
        //
        switch(position)
        {
            case 0:
                llc[0]      = value;
                position    = 1;
                break;

            case 1:
                llc[1]      = value;
                position    = 2;
                break;

            case 2:
            {
                llc[2]      = value;
                uint32_t    field   = (llc[0]<<16) | (llc[1]<<8) | (llc[2]);
                ProcessLLC( field );
                position    = 3;
                break;
            }   




            case 3:
                requestTag      = value;
                position    = 4;
                break;

            case 4:
            {
                requestLength      = value;
                ProcessRequestType( requestTag, requestLength );
                position    = 5;
                break;
            }

            case 5:
            {
                invokeId          = value;
                ProcessInvokeId( invokeId );
                position    = 6;
                break;
            }

            case 6:
                classId[0]      = value;
                position    = 7;
                break;

            case 7:
            {
                classId[1]      = value;
                uint16_t    field   = (classId[0]<<8) | (classId[1]);
                ProcessInterfaceClass( field );
                position    = 8;
                break;
            }

            case 8:
                logicalName.d0      = value;
                position    = 9;
                break;

            case 9:
                logicalName.d1      = value;
                position    = 10;
                break;

            case 10:
                logicalName.d2      = value;
                position    = 11;
                break;

            case 11:
                logicalName.d3      = value;
                position    = 12;
                break;

            case 12:
                logicalName.d4      = value;
                position    = 13;
                break;

            case 13:
            {
                logicalName.d5      = value;
                ProcessLogicalName( logicalName );
                position    = 14;
                break;
            }

            case 14:
                attributeNumber[0]   = value;
                position    = 15;
                break;

            case 15:
            {
                attributeNumber[1]   = value;
                uint16_t    field   = (attributeNumber[0]<<8) | (attributeNumber[1]);
                ProcessAttributeNumber( field );
                position    = 16;
                break;
            }

            default:
                break;
        }

        return true;
    }

private:


};




//
//
//
class HDLCFrame
{

public:

    HDLCFrame( ByteStream& _stream, DLMSParser& _upperLayer ) :
        stream( _stream ),
        upperLayer( _upperLayer )
    {
    }


    bool Get_UINT8( uint8_t& value )
    {
        bool            success     = true;
        static bool     withinFrame = false;
        static uint8_t  fcs[2];
        static uint16_t calculatedFCS;
        
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
                    calculatedFCS   = 0xffff;
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

                //
                // complete the FCS calculation.
                //
                calculatedFCS = ~calculatedFCS;
                calculatedFCS = ((calculatedFCS >> 8) & 0xFF) | (calculatedFCS << 8);

                printf("calculatedFCS = %04x\n", calculatedFCS);

                //
                // Check the FCS.
                //
                printf("FCS = %02x%02x\n", fcs[0],fcs[1]);

                uint16_t    field   = (fcs[0]<<8) | (fcs[1]);
                if(field == 0x0dfd)
                {
                    upperLayer.Commit();
                }
                else
                {
                    upperLayer.Abort();
                }
            }

            //
            // update the FCS.
            //
            calculatedFCS = (calculatedFCS >> 8) ^ crc16Table[(calculatedFCS ^ value) & 0xFF];
        }

        //
        // Update the FCS (shift along the received FCS and calculate the current one).
        //
        fcs[0]  = fcs[1];
        fcs[1]  = value;

        return success;
    }


    bool IsIFrame(uint8_t frameType)
    {
        uint8_t     type    = frameType & 0x03;
        if( type == 0 )
        {
            return true;
        }
        if( type == 2 )     // TODO: is this correct?
        {
            return true;
        }

        return false;
    }
    

    bool IsSFrame(uint8_t frameType)
    {
        uint8_t     type    = frameType & 0x03;
        if( type == 1 )
        {
            return true;
        }

        return false;
    }
    

    bool IsUFrame(uint8_t frameType)
    {
        uint8_t     type    = frameType & 0x03;
        if( type == 3 )
        {
            return true;
        }

        return false;
    }
    

    void ProcessFrameType( uint8_t frameType )
    {
        printf("FrameType = %02x\n", frameType);
        uint8_t     type    = frameType & 0x03;

        switch( type )
        {
            case 0:
                printf("\tI-Frame (user data)\n");
                break;

            case 1:
                printf("\tS-Frame (flow control)\n");
                break;

            case 3:
                printf("\tU-Frame (link mgmt, SNRM & UA)\n");
                break;

        }
    }
    
    bool Parse( uint8_t value )
    {
        bool                success     = false;
        static uint32_t     position    = 0;        
        static uint8_t      header[7];
        static uint8_t      hcs[2];
        static uint8_t      type;
        static uint16_t     frameLength;
        static uint8_t      addressField[4];
        static uint8_t      addressFieldSize;
        static uint8_t      addressCount;
        static uint8_t      frameType;


        //
        // Parse the header bytes.
        //
        switch(position)
        {
            case 0:
                type            = value & 0xf0;
                frameLength     = (value&0x7)<<8;
                printf("Type=%02x\n", type);
                addressCount    = 0;
                position        = 1;
                break;



            case 1:
                frameLength += value;
                printf("FrameLength=%04x\n", frameLength);
                position        = 3;
                break;


            case 3:
                addressField[0]   = value;
                if( (value & 0x01) != 0)
                {
                    addressFieldSize    = 1;
                    printf("a1=%02x\n",addressField[0]);
                    position            = 15;
                }
                else
                {
                    position        = 4;
                }
                break;

            case 4:
                addressField[1]     = value;
                if( (value & 0x01) != 0)
                {
                    addressFieldSize    = 2;
                    printf("a1=%02x%02x\n",addressField[0],addressField[1]);
                    position            = 15;
                }
                else
                {
                    position        = 5;
                }
                break;


            case 5:
                addressField[2]     = value;
                addressFieldSize    = 4;
                position            = 6;
                break;

            case 6:
                addressField[3]     = value;
                addressFieldSize    = 4;
                printf("a1=%02x%02x%02x%02x\n",addressField[0],addressField[1],addressField[2],addressField[3]);
                position            = 15;
                break;




            case 15:
                addressField[0]   = value;
                if( (value & 0x01) != 0)
                {
                    addressFieldSize    = 1;
                    printf("a2=%02x\n",addressField[0]);
                    position            = 27;
                }
                else
                {
                    position        = 16;
                }
                break;

            case 16:
                addressField[1]   = value;
                if( (value & 0x01) != 0)
                {
                    addressFieldSize    = 2;
                    printf("a2=%02x%02x\n",addressField[0],addressField[1]);
                    position            = 27;
                }
                else
                {
                    position        = 17;
                }
                break;

            case 17:
                addressField[2]     = value;
                addressFieldSize    = 4;
                position            = 18;
                break;

            case 18:
                addressField[3]     = value;
                addressFieldSize    = 4;
                printf("a2=%02x%02x%02x%02x\n",addressField[0],addressField[1],addressField[2],addressField[3]);
                position            = 27;
                break;







            case 27:
                frameType      = value;
                ProcessFrameType( frameType );
                position    = 28;
                break;

            case 28:
                hcs[0]          = value;
                position    = 29;
                break;

            case 29:
                hcs[1]          = value;
                printf("HCS = %02x%02x\n", hcs[0],hcs[1]);
                if( IsIFrame( frameType ) == true)
                {
                    position    = 30;
                }
                if( IsSFrame( frameType ) == true)
                {
                    position    = 40;
                }
                if( IsUFrame( frameType ) == true)
                {
                    position    = 50;
                }
                break;



            case 30:        // I-Frame
                printf("I-Frame.\n");
                upperLayer.Parse( value );
                break;

            case 40:        // S-Frame
                printf("S-Frame.\n");
                break;

            case 50:        // U-Frame
                printf("U-Frame.\n");
                break;


            default:
                printf("<fault>\n");
                break;
        }

        return true;
    }


private:

    static const uint8_t    HDLC_FRAME_DELIMIT     = 0x7E;
    static const uint16_t   crc16Table[256];

    ByteStream&     stream;
    DLMSParser&     upperLayer;

};



const uint16_t   HDLCFrame::crc16Table[] =
{
    0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF,
    0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7,
    0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E,
    0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876,
    0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD,
    0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5,
    0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C,
    0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974,
    0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB,
    0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3,
    0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A,
    0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72,
    0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9,
    0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1,
    0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738,
    0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70,
    0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7,
    0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
    0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036,
    0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E,
    0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5,
    0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD,
    0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134,
    0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C,
    0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3,
    0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB,
    0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232,
    0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A,
    0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1,
    0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9,
    0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330,
    0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78
};




//
//
//
int main()
{
    printf("DLMS encode/decoder test\n");

    {
        //
        // HDLC frame start: 7E
        // frame type: A0
        // frame length: 19
        // dest address: 95 
        // source address upper: 75,54,
        // source address lower: 68,35
        // llc bytes: E6E600
        // GET request normal: C001
        // invoke id and priority: 81
        // interface class 8, clock: 0008
        // logical name, obis code of clock: 0000010000FF
        // asking for 1st attribute, logical name: 0100
        // HDLC frame check sequence: 0DFD
        // HDLC frame close: 7E
        //
        uint8_t     snrmRequest[]       = {0x7E,0xA0,0x0A,0x00,0x02,0x00,0x23,0x21,0x93,0x18,0x71,0x7E};
        uint8_t     uaRequest[]         = {0x7E,0xA0,0x23,0x21,0x00,0x02,0x00,0x23,0x73,0xF6,0xC5,0x81,0x80,0x14,0x05,0x02,0x00,0x80,0x06,0x02,0x00,0x80,0x07,0x04,0x00,0x00,0x00,0x01,0x08,0x04,0x00,0x00,0x00,0x01,0xCE,0x6A,0x7E};
        uint8_t     aarqRequest[]       = {0x7E,0xA0,0x2E,0x00,0x02,0x00,0x23,0x21,0x10,0x7E,0xCB,0xE6,0xE6,0x00,0x60,0x1D,0xA1,0x09,0x06,0x07,0x60,0x85,0x74,0x05,0x08,0x01,0x01,0xBE,0x10,0x7E,0xA0,0x2E,0x00,0x02,0x00,0x23,0x21,0x10,0x7E,0xCB,0xE6,0xE6,0x00,0x60,0x1D,0xA1,0x09,0x06,0x07,0x60,0x85,0x74,0x05,0x08,0x01,0x01,0xBE,0x10};
        uint8_t     readClockRequest[]  = {0x7E,0xA0,0x19,0x95,0x75,0x54,0x68,0x35,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x08,0x00,0x00,0x01,0x00,0x00,0xFF,0x01,0x00,0x0D,0xFD,0x7E};
        uint8_t     readClockResponse[] = {0x7E,0xA0,0x18,0x75,0x95,0x74,0xE9,0xE8,0xE6,0xE7,0x00,0xC4,0x01,0x81,0x00,0x09,0x06,0x00,0x00,0x01,0x00,0x00,0xFF,0xFD,0x49,0x7E};
        uint8_t     readClockResponse2[]= {0x7E,0xA0,0x1E,0x75,0x95,0x96,0x6F,0x67,0xE6,0xE7,0x00,0xC4,0x01,0x81,0x00,0x09,0x0C,0x07,0xD2,0x0C,0x04,0x03,0x0A,0x06,0x0B,0xFF,0x00,0x78,0x00,0xF3,0x30,0x7E};
        //ByteStream  requestStream( &readClockRequest[0], sizeof(readClockRequest) );
        //ByteStream  requestStream( &aarqRequest[0], sizeof(aarqRequest) );
        //ByteStream  requestStream( &uaRequest[0], sizeof(uaRequest) );
        //ByteStream  requestStream( &snrmRequest[0], sizeof(snrmRequest) );
        //ByteStream  requestStream( &readClockResponse[0], sizeof(readClockResponse) );
        ByteStream  requestStream( &readClockResponse2[0], sizeof(readClockResponse2) );
        DLMSParser  dlmsParser;
        HDLCFrame   requestFrame( requestStream, dlmsParser );

        bool        success             = false;

        //
        // Parse the frame.
        //
        do
        {
            uint8_t     value;
            success     = requestFrame.Get_UINT8( value );
            if( success == true)
            {
                success = requestFrame.Parse( value );
            }

        } while( success == true );
    }

    printf("\n");
}






