//
// uint32 encoded using a continuation-bit, 0 indicates another byte to come, 1 indicates no more
// bytes.
// This scheme means there is an advantage gained while storing values closer to zero as less bytes
// need to be stored to represent the data.
// If the numeric value is combined with a set of flags, the flags should be in the LSB, meaning they
// do not create an artificially large-valued number.
//
// Timestamps:
// up to uint32-max in total, Type in high part of timestamp.
// First uint32 in packet is a time-base. All other timestamps are deltas from this. 
// Resolution is 0.1s
// time can use lower 2048 (204.8sec, 11 bits), leaving upper 4 bits for type and 1 for continuation-bit.
// 0123456701234567
// 0TTTTttttttttttt
//
// Type 0: marker
// <markerAndType:uint32>
// 01234567
// 1xxxxxtt   x = marker-value, t = type.
// typical size 1b
//
// Type 1: hex dump:
// <numberOfBytesinBLOB&Type> <BLOB>
// 01234567
// 1nnnnntt     n = number-of-bytes, t = type
// 1 byte can encode up to32 bytes of data.
// typical size 1 + n 
//
// Type 2: printf
// <formatAddressAndFlags:uint32> <:uint32> <:uint32> ...
// 012345670123456701234567
// 0aaaaaaa0aaaaaaa1aaaaatt     a=address t = type.
// 3 bytes can encode any string address in a 512KB image (19-bits)
// parameters will follow as specified by the format-string.
//
// Type 3: Timestamp
//
// In typical 256b packet:
// 256/1 = 256 type 0.
// 256/(1+31) = 8 type 1.
// 256/3 = 85 type 1 with no params.
// 256/(3+2) = 51 type 1 with 2 small params.
//



#include "Trace.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdlib.h>

// Masks for working out the 7-bit portions.
#define MASK_B0                         (0x0000007f)
#define MASK_B1                         (0x00003f80)
#define MASK_B2                         (0x001fc000)
#define MASK_B3                         (0x0fe00000)
#define MASK_B4                         (0xf0000000)
#define SHIFT_B0                        (0)
#define SHIFT_B1                        (7)
#define SHIFT_B2                        (14)
#define SHIFT_B3                        (21)
#define SHIFT_B4                        (28)


// Packet & stream data.
uint8_t     tracePacket[256];
uint8_t*    tracePacketPtr  = NULL;
uint32_t    totalSize       = 0;

uintptr_t   rodataBase  = 4464;
uint8_t*    image       = NULL;



//
void traceDecodeUInt8( uint8_t** ptr, uint8_t* value, bool* lastFlag )
{
    // Copy the values out of the stream.
    *value      = **ptr;
    *lastFlag   = false;

    // If the top bit is set, this is the last byte in the value.
    if( ((*value)&0x80) != 0 )
    {
        *lastFlag   =  true;
    }

    // Clear the top bit as that carries a control value, not data.
    *value  = *value & 0x7f;

    // Move the stream pointer along.
    *ptr = (*ptr) + 1;
}

//
void traceEncodeUInt8( uint8_t value, uint8_t** ptr, bool lastFlag )
{
    // Set the top bit if its the last in the sequence, clear it otherwise.
    value   &= 0x7f;
    if( lastFlag == true )
    {
        value   |= 0x80;
    }

    // Insert the value into the byte stream.
    **ptr   = value;
    
    // Move the head-of-stream pointer along.
    *ptr    = (*ptr) + 1;
}

//
// Encode a 29-bit value.
//
void traceEncodeUInt32( uint32_t value, uint8_t** ptr )
{
    if( value < 0x7f )
    {
        // 7-bit case.
        traceEncodeUInt8( (value & MASK_B0) >> SHIFT_B0, ptr, true );
    }
    else if( value < 0x7fff )
    {
        // 14-bit case.
        traceEncodeUInt8( (value & MASK_B1) >> SHIFT_B1, ptr, false );
        traceEncodeUInt8( (value & MASK_B0) >> SHIFT_B0, ptr, true );
    }
    else if( value < 0x7fffff )
    {
        // 21-bit case.
        traceEncodeUInt8( (value & MASK_B2) >> SHIFT_B2, ptr, false );
        traceEncodeUInt8( (value & MASK_B1) >> SHIFT_B1, ptr, false );
        traceEncodeUInt8( (value & MASK_B0) >> SHIFT_B0, ptr, true );
    }
    else if( value < 0xfffffff )
    {
        // 28-bit case.
        traceEncodeUInt8( (value & MASK_B3) >> SHIFT_B3, ptr, false );
        traceEncodeUInt8( (value & MASK_B2) >> SHIFT_B2, ptr, false );
        traceEncodeUInt8( (value & MASK_B1) >> SHIFT_B1, ptr, false );
        traceEncodeUInt8( (value & MASK_B0) >> SHIFT_B0, ptr, true );
    }
    else
    {
        // 32-bit case.
        traceEncodeUInt8( (value & MASK_B4) >> SHIFT_B4, ptr, false );
        traceEncodeUInt8( (value & MASK_B3) >> SHIFT_B3, ptr, false );
        traceEncodeUInt8( (value & MASK_B2) >> SHIFT_B2, ptr, false );
        traceEncodeUInt8( (value & MASK_B1) >> SHIFT_B1, ptr, false );
        traceEncodeUInt8( (value & MASK_B0) >> SHIFT_B0, ptr, true );
    }
}

//
void traceEncodeZeroTerminatedBLOB( uint8_t* blob, uint32_t numberOfBytes, uint8_t** ptr )
{
    // copy the string into the output.
    for( uint32_t i=0; i<numberOfBytes; i++ )
    {
        uint8_t     c   = blob[i];
        **ptr       = c;
        *ptr        += 1;
        **ptr       = 0;
    }

    // Don't forget about the zero-terminator.
    **ptr       = 0;
    *ptr        += 1;
}

//
void traceDecodeZeroTerminatedBLOB( uint8_t* blob, uint32_t maxNumberOfBytes, uint8_t** ptr )
{
    for( uint32_t i=0; i<maxNumberOfBytes; i++ )
    {
        char    c   = **ptr;
        if( c != 0 )
        {
            blob[i] = c;
        }
        else
        {
            blob[i] = 0;
            break;
        }

        *ptr    += 1;
    }
}

//
void traceEncodeFixedSizeBLOB( uint8_t* blob, uint32_t numberOfBytes, uint8_t** ptr )
{
    memcpy( *ptr, blob, numberOfBytes );
    *ptr    += numberOfBytes;
}

//
void traceDecodeFixedSizeBLOB( uint8_t* blob, uint32_t numberOfBytes, uint8_t** ptr )
{
    memcpy( blob, *ptr, numberOfBytes );
    *ptr    += numberOfBytes;
}


//
void traceEncodeBLOB( uint8_t* blob, uint32_t numberOfBytes, uint8_t** ptr )
{
    // For every block of 7-bits in the BLOB, encode the byte.
    uint32_t    numberOfBitsEncoded = 0;
    for(uint32_t bitPosition=0; bitPosition<numberOfBytes*8; bitPosition+=7)
    {
        // Any single block of 7-bits may straddle a byte boundary,
        // therefore we need to work out where the uint16_t value
        // lies that can contain it completely and the corresponding
        // mask for the value itself.
        uint32_t    bytePosition                = bitPosition / 8;
        uint32_t    bytePositionInBits          = bytePosition * 8;
        uint32_t    offsetOfFirstBitIntoByte    = bitPosition - bytePositionInBits;
        uint16_t    mask                        = 0x007f << offsetOfFirstBitIntoByte;
        uint16_t    value16                     = *((uint16_t*)(blob+bytePosition)) & mask;
        uint8_t     value8                      = value16 >> offsetOfFirstBitIntoByte;

        // Is this the last one?
        bool    lastFlag    = false;

        // Output the 7-bits as an encoded byte.
        traceEncodeUInt8( (uint32_t)value8, ptr, lastFlag );
    }
}


//
void traceDecodeBLOB( uint8_t* blob, uint32_t maxNumberOfBytes, uint8_t** ptr )
{
    bool        lastFlag        = true;
    uint32_t    numberOfBits    = 0;

    do
    {
        uint8_t value       = 0;

        traceDecodeUInt8( ptr, &value, &lastFlag );
        uint32_t    byteNumber  = numberOfBits / 8;

        // next one...
        numberOfBits    += 7;

    } while( lastFlag == false );
}

//
void traceDecodeUInt32( uint32_t* value, uint8_t** ptr )
{
    bool        lastFlag    = false;
    
    // Must start off with no bits set.
    *value  = 0;

    // Repeatedly read 7 bits from the stream until we get to
    // a byte with the top bit set, then stop.
    do
    {
        // Shift up the previous value by 7 bits (we start off with 0 so
        // this is harmless initially).
        *value  = (*value) << 7;

        // Get the next byte out.
        uint8_t     byteValue   = 0;
        traceDecodeUInt8( ptr, &byteValue, &lastFlag );

        // Combine the new byte with the existing value.
        *value  = (*value) | byteValue;

    } while( lastFlag == false );
}







const char* decodeConstantStringPointer( uint32_t encodedValue )
{
    uintptr_t   address = (uintptr_t)encodedValue;
    address += rodataBase;
    const char* text    = (const char*)&image[address];
    return text;
}


void traceDecodePrintf( uint8_t** ptr, char* output, uint32_t maxOutputSize, const char* format )
{
    // ...then scan thru the string finding all the
    // format specifiers, determine their size and output
    // the binary data associated with them.
    bool    percent = false;
    for( uint32_t i=0; i<strlen(format); i++)
    {
        if( format[i] == '%' )
        {
            percent = true;
        }
        else if( percent == true )
        {
            char    type            = format[i];
            char    fieldText[64]   = {0};
            char    formatText[8]   = {0};

            //
            formatText[0]   = '%';
            formatText[1]   = type;
            formatText[2]   = 0;

            //
            switch(type)
            {
                case 'e':
                case 'f':
                case 'g':
                {
                    double  fValue      = 0;
                    traceDecodeFixedSizeBLOB( (uint8_t*)&fValue, sizeof(fValue),  ptr );
                    snprintf( &fieldText[0], sizeof(fieldText), formatText, fValue );
                    break;
                }

                case 's':
                {
                    uint8_t sValue[64];
                    traceDecodeZeroTerminatedBLOB( (uint8_t*)&sValue, sizeof(sValue),  ptr );
                    snprintf( &fieldText[0], sizeof(fieldText), formatText, sValue );
                    break;
                }

                case 'c':
                case 'd':
                case 'o':
                case 'p':
                case 'u':
                case 'x':
                case 'z':
                {
                    uint32_t    value;
                    traceDecodeUInt32( &value, ptr );
                    snprintf( &fieldText[0], sizeof(fieldText), formatText, value );
                    break;
                }

                default:
                     traceEncodeUInt32( 0, ptr );
                    break;
            }

            //
            strcat( &output[0], &fieldText[0] );

            // For now, we only parse simple format-specifiers, i.e "%d".
            percent = false;
        }
        else 
        {
            uint32_t    currentLength   = strlen(output);
            output[ currentLength+0 ]    = format[i];
            output[ currentLength+1 ]    = 0;
        }
    }
}



//
void traceDecode( uint8_t** ptr )
{
    char        text[128] = {0};
    uint32_t    value   = 0;
    traceDecodeUInt32( &value, ptr );

    uint8_t     type    = value & TYPE_MASK;
    value >>= BITS_PER_TYPE;
    
    // deserialise the text.
    switch( type )
    {
        case Marker:
            // Marker
            snprintf( &text[0], sizeof(text), "marker %d", value);
            break;

        case HexDump:
        {
            // Hex/binary data.
            uint32_t    numberOfBytes   = value;
            static uint8_t  data[128]   = {0};
            memcpy( &data[0], *ptr, numberOfBytes );
            *ptr    += numberOfBytes;
            snprintf( &text[0], sizeof(text), "%d bytes: ", numberOfBytes);
            char    value[8];
            for( uint32_t i=0; i<numberOfBytes; i++) 
            {
                snprintf( &value[0], sizeof(value), "%02x ",data[i] );
                strcat( &text[0], &value[0] );
            }
            break;
        }

        case PrintF:
        {
            // Serialised printf.
            const char*   pAddress    = decodeConstantStringPointer( value );
            traceDecodePrintf( ptr, &text[0], sizeof(text), pAddress );
            break;
        }

        case CachedBLOB:
            break;

        default:
            break;
    }

    // output the text.
    totalSize   += strlen(text);
    puts( text );
}




//
int main( int argc, char* argv[] )
{
    FILE*       imageFile   = NULL;

    //
    imageFile   = fopen( argv[1], "rb" );
    fseek( imageFile, 0L, SEEK_END );
    long length = ftell( imageFile );
    fseek( imageFile, 0L, SEEK_SET );
    image   = (uint8_t*)malloc( length );
    fread( image, length, 1, imageFile );
    
    printf("length = %ld\n", length);
    printf("[%s]\n",&image[rodataBase]);

    //
    uint32_t    serialisedSize  = 0;
    while( feof(stdin) == 0 ) 
    {
        uint8_t     hi  = fgetc(stdin);
        uint8_t     lo  = fgetc(stdin);
        printf("%c%c ",hi,lo);
        hi  = (hi <= '9' ? hi-'0' : hi-'a' );
        lo  = (lo <= '9' ? lo-'0' : lo-'a' );
        uint8_t     value   = (hi<<4) | lo;

        tracePacket[serialisedSize] = value;
        serialisedSize++;
    }

    printf("\n[");
    for(uint32_t i=0; i<serialisedSize; i++)
    {
        printf("%02x ",tracePacket[i]);
    }
    printf("]\n");

    // Decode
    tracePacketPtr  = &tracePacket[0];

    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );
    traceDecode( &tracePacketPtr );

    printf("serialised size = %"PRIu32"\n", serialisedSize);
    printf("deserialised size = %d\n", totalSize);
    printf("serialise size = %.1f%%\n",(100.0/totalSize)*(float)serialisedSize);
}


