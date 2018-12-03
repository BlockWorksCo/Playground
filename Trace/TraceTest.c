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
void traceEncodeFixedSizeBLOB( uint8_t* blob, uint32_t numberOfBytes, uint8_t** ptr )
{
    memcpy( *ptr, blob, numberOfBytes );
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
void traceEncodeHex( uint8_t* data, uint32_t numberOfBytes, uint8_t** ptr )
{
    // Encode the type with the numberOfBytes.
    uint32_t    type    = HexDump;
    uint32_t    value   = (numberOfBytes << BITS_PER_TYPE) | type;
    traceEncodeUInt32( value, ptr );

    // Then simply copy the bytes into the stream.
    traceEncodeFixedSizeBLOB( data, numberOfBytes, ptr );
}




//
void traceEncodeTruncatedHex( uint8_t* data, uint32_t numberOfBytes, uint8_t** ptr )
{
    // Limit the number of bytes we actually output. This will be decoded
    // with an ellipsis appended to it "..." to indicate this.
    static const uint32_t   truncationLimit = 8;
    if( numberOfBytes > truncationLimit )
    {
        numberOfBytes   = truncationLimit;
    }

    // Encode the type with the numberOfBytes.
    uint32_t    type    = TruncatedBLOB;
    uint32_t    value   = (numberOfBytes << BITS_PER_TYPE) | type;
    traceEncodeUInt32( value, ptr );

    // Then simply copy the bytes into the stream.
    traceEncodeFixedSizeBLOB( data, numberOfBytes, ptr );
}


//
void traceEncodeMarker( uint32_t marker, uint8_t** ptr )
{
    uint32_t    type    = Marker;
    uint32_t    value   = (marker << BITS_PER_TYPE) | type;

    traceEncodeUInt32( value, ptr );
}


uint32_t encodeConstantStringPointer( const char* text )
{
    extern const void * const rodata_start;
    uintptr_t   rodataBase  = (uintptr_t)&rodata_start;

    // first, determine the address of the format string identifier...
    // minus the base address.
    uintptr_t  address = (uintptr_t)text;
    address     -= rodataBase;

    //printf("%"PRIiPTR" %"PRIiPTR" = [%s]\n",rodataBase,address,text);

    uint32_t    encodedValue    = (uint32_t)address;
    return encodedValue;
}


//
// %c char single character
// %d (%i) int signed integer
// %e (%E) float or double exponential format
// %f float or double signed decimal
// %g (%G) float or double use %f or %e as required
// %o int unsigned octal value
// %p pointer address stored in pointer
// %s array of char sequence of characters
// %u int unsigned decimal
// %x (%X) int unsigned hex value
// %z size_t.
//
//src/platform/core/jprintf.h
//13: *  - %p  - Pointer
//14: *  - %p6 - Pointer to IPadr
//15: *  - %p4 - Pointer to IPv4adr
//16: *  - %pi - Pointer to IID
//17: *  - %pm - Pointer to 6 bytes (Ethernet EUI-48 MAC address)
//18: *  - %pM - Pointer to 8 bytes (IEEE 802.15.4 EUI-64 address)
//19: *  - %po - Pointer to 6 bytes (OBIS code)
//
void traceEncodePrintf( uint8_t** ptr, const char* format, ... )
{
    va_list args;
    va_start(args, format);

    // first, determine the address of the format string identifier...
    // minus the base address.
    uint32_t    address = encodeConstantStringPointer( format );

    // Encode the type with the address.
    uint32_t    type    = PrintF;
    uint32_t    value   = (address << BITS_PER_TYPE) | type;
    traceEncodeUInt32( value, ptr );

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
            char    type    = format[i];

            //
            switch(type)
            {
                case 'e':
                case 'f':
                case 'g':
                {
                    double  fValue  = va_arg( args, double );
                    traceEncodeFixedSizeBLOB( (uint8_t*)&fValue, sizeof(fValue), ptr );
                    break;
                }

                case 's':
                {
                    uint8_t*    sValue  = va_arg( args, char* );
                    traceEncodeZeroTerminatedBLOB( &sValue[0], strlen(sValue), ptr );
                    break;
                }  

                case 'c':
                case 'd':
                case 'o':
                case 'u':
                case 'x':
                case 'z':
                {
                    traceEncodeUInt32( (uint32_t)va_arg(args,uint32_t), ptr );
                    break;
                }

                case 'p':
                {
                    uint8_t     subType = format[i+1];
                    uint32_t    subTypeLength   = 4;

                    // Decide on how many bytes represent each display type.
                    switch( subType )
                    {
                        case '6':   subTypeLength   = 16; break;
                        case '4':   subTypeLength   = 4; break;
                        case 'i':   subTypeLength   = 16; break;
                        case 'm':   subTypeLength   = 6; break;
                        case 'M':   subTypeLength   = 8; break;
                        case 'o':   subTypeLength   = 6; break;
                    }

                    // Only put that many byte into the output stream.
                    uint8_t*    pValue  = va_arg( args, char* );
                    traceEncodeFixedSizeBLOB( pValue, subTypeLength, ptr );
                    break;
                }

                default:
                     traceEncodeUInt32( 0, ptr );
                    break;
            }

            // For now, we only parse simple format-specifiers, i.e "%d".
            percent = false;
        }
    }

    va_end(args);
}






//
int main()
{
    // Encode
    tracePacketPtr  = &tracePacket[0];

    traceEncodeMarker( 1, &tracePacketPtr );
    traceEncodeMarker( 2, &tracePacketPtr );
    traceEncodeMarker( 3, &tracePacketPtr );
    traceEncodePrintf( &tracePacketPtr, "Hello World." );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%d)", 123 );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%d, %d)", 456,789 );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%x, %x, %x)", 0xab,0xabcd, 0x0123abcd );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%c, %f, %g)", 'A',3.14, 304.0 );
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%s)", "Blaa!" );
    traceEncodePrintf( &tracePacketPtr, "This is a long message to see if it improves the compression ratio....");
    uint8_t data[]  = {0x12,0x34,0x45,0x67,0x89,0xab,0xcd,0xef};
    traceEncodeHex( &data[0], sizeof(data), &tracePacketPtr );
    uint8_t bigBLOB[]  = {0x12,0x34,0x45,0x67,0x89,0xab,0xcd,0xef, 0x01,0x35,0x46,0x10,0x19,0x23};
    traceEncodeTruncatedHex( &bigBLOB[0], sizeof(bigBLOB), &tracePacketPtr );
    uint8_t ipv6Addr[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    traceEncodePrintf( &tracePacketPtr, "Hello World. (%p6)", ipv6Addr );

    ptrdiff_t   serialisedSize    = tracePacketPtr - &tracePacket[0];
    //printf("\n[");
    for(uint32_t i=0; i<serialisedSize; i++) 
    {
        printf("%02x",tracePacket[i]);
    }
    //printf("]\n");
}


