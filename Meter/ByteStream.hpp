


#ifndef __BYTESTREAM_HPP__
#define __BYTESTREAM_HPP__



#include <stdint.h>
#include <stdbool.h>


//
//
//
class ByteStream
{

public:

    ByteStream(uint8_t* _data, uint32_t _numberOfBytes);

    bool Get_UINT8( uint8_t& value )

private:

    uint8_t*    data;
    uint32_t    numberOfBytes;
    uint32_t    currentPosition;

};


#endif

