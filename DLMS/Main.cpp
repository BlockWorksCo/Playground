


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
        printf("LLC: %08x\n", value);
    }

    bool ProcessRequestType(uint16_t value)
    {
        printf("RequestType: %04x\n", value);
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
        static uint8_t      requestType[2];
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
                requestType[0]      = value;
                position    = 4;
                break;

            case 4:
            {
                requestType[1]      = value;
                uint16_t    field   = (requestType[0]<<8) | (requestType[1]);
                ProcessRequestType( field );
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

                //
                // Check the FCS.
                //
                printf("FCS = %02x%02x\n", fcs[0],fcs[1]);
            }
        }

        //
        // Update the FCS (shift along the received FCS and calculate the current one).
        //
        fcs[0]  = fcs[1];
        fcs[1]  = value;

        return success;
    }

    
    
    bool Parse( uint8_t value )
    {
        bool                success     = false;
        static uint32_t     position    = 0;        
        static uint8_t      header[7];
        static uint8_t      hcs[2];
        static uint8_t      type;
        static uint16_t     frameLength;
        static uint8_t      addressField[2];
        static uint8_t      addressFieldSize;
        static uint8_t      addressCount;
        static uint8_t      controlByte;


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
                    position            = 5;
                }
                else
                {
                    position        = 4;
                }
                break;

            case 4:
                addressField[1]     = value;
                addressFieldSize    = 2;
                printf("a1=%02x%02x\n",addressField[0],addressField[1]);
                position        = 5;
                break;

            case 5:
                addressField[0]   = value;
                if( (value & 0x01) != 0)
                {
                    addressFieldSize    = 1;
                    printf("a2=%02x\n",addressField[0]);
                    position            = 7;
                }
                else
                {
                    position        = 6;
                }
                break;

            case 6:
                addressField[1]   = value;
                addressFieldSize    = 2;
                printf("a2=%02x%02x\n",addressField[0],addressField[1]);
                position            = 7;
                break;




            case 7:
                controlByte      = value;
                printf("ControlByte = %02x\n", controlByte);
                position    = 8;
                break;

            case 8:
                hcs[0]          = value;
                position    = 9;
                break;

            case 9:
                hcs[1]          = value;
                printf("HCS = %02x%02x\n", hcs[0],hcs[1]);
                position    = 10;
                break;





            case 24:
            {
                fcs[1]   = value;
                uint16_t    field   = (fcs[0]<<8) | (fcs[1]);
                if(field == 0x0dfd)
                {
                    upperLayer.Commit();
                }
                else
                {
                    upperLayer.Abort();
                }
                break;
            }

            default:
                upperLayer.Parse( value );
                position++;
                break;
        }

        return true;
    }


private:

    static const uint8_t    HDLC_FRAME_DELIMIT     = 0x7E;

    ByteStream&     stream;
    DLMSParser&     upperLayer;

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
        uint8_t     readClockRequest[] = {0x7E,0xA0,0x19,0x95,0x75,0x54,0x68,0x35,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x08,0x00,0x00,0x01,0x00,0x00,0xFF,0x01,0x00,0x0D,0xFD,0x7E};
        ByteStream  requestStream( &readClockRequest[0], sizeof(readClockRequest) );
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






