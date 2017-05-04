


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
    void AbortProcessing()
    {
        printf("Abort\n");
    }

    void CompleteProcessing()
    {
        printf("Complete\n");
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

    
    
    bool Parse()
    {
        bool        success     = false;
        uint32_t    position    = 0;        
        uint8_t     header[7];
        uint8_t     fcs[2];
        uint8_t     llc[4];
        uint8_t     requestType[2];
        LogicalName logicalName;
        uint8_t     invokeId;
        uint8_t     classId[2];
        uint8_t     attributeNumber[2];

        do
        {
            uint8_t     value;
            success     = Get_UINT8( value );
            if( success == true )
            {
                //
                // Parse the header bytes.
                //
                switch(position)
                {
                    case 0:
                        header[0]   = value;
                        break;

                    case 1:
                        header[1]   = value;
                        break;

                    case 2:
                        header[2]   = value;
                        break;

                    case 3:
                        header[3]   = value;
                        break;

                    case 4:
                        header[4]   = value;
                        break;

                    case 5:
                        header[5]   = value;
                        break;

                    case 6:
                        header[6]   = value;
                        break;

                    case 7:
                        llc[0]      = value;
                        break;

                    case 8:
                        llc[1]      = value;
                        break;

                    case 9:
                    {
                        llc[2]      = value;
                        uint32_t    field   = (llc[0]<<16) | (llc[1]<<8) | (llc[2]);
                        upperLayer.ProcessLLC( field );
                        break;
                    }   

                    case 10:
                        requestType[0]      = value;
                        break;

                    case 11:
                    {
                        requestType[1]      = value;
                        uint16_t    field   = (requestType[0]<<8) | (requestType[1]);
                        upperLayer.ProcessRequestType( field );
                        break;
                    }

                    case 12:
                    {
                        invokeId          = value;
                        upperLayer.ProcessInvokeId( invokeId );
                        break;
                    }

                    case 13:
                        classId[0]      = value;
                        break;

                    case 14:
                    {
                        classId[1]      = value;
                        uint16_t    field   = (classId[0]<<8) | (classId[1]);
                        upperLayer.ProcessInterfaceClass( field );
                        break;
                    }

                    case 15:
                        logicalName.d0      = value;
                        break;

                    case 16:
                        logicalName.d1      = value;
                        break;

                    case 17:
                        logicalName.d2      = value;
                        break;

                    case 18:
                        logicalName.d3      = value;
                        break;

                    case 19:
                        logicalName.d4      = value;
                        break;

                    case 20:
                    {
                        logicalName.d5      = value;
                        upperLayer.ProcessLogicalName( logicalName );
                        break;
                    }

                    case 21:
                        attributeNumber[0]   = value;
                        break;

                    case 22:
                    {
                        attributeNumber[1]   = value;
                        uint16_t    field   = (attributeNumber[0]<<8) | (attributeNumber[1]);
                        upperLayer.ProcessAttributeNumber( field );
                        break;
                    }

                    case 23:
                        fcs[0]   = value;
                        break;

                    case 24:
                    {
                        fcs[1]   = value;
                        uint16_t    field   = (fcs[0]<<8) | (fcs[1]);
                        if(field == 0x0dfd)
                        {
                            upperLayer.CompleteProcessing();
                        }
                        else
                        {
                            upperLayer.AbortProcessing();
                        }
                        break;
                    }

                    default:
                        break;
                }

                position++;
            }

        } while( success == true );

        return false;
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
            success = requestFrame.Parse();

        } while( success == true );
    }

    printf("\n");
}






