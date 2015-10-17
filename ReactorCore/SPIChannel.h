



#ifndef __SPICHANNEL_H__
#define __SPICHANNEL_H__




//
//
//
template <  uint32_t baseAddress, 
            uint32_t outputBufferSize,
            uint32_t inputBufferSize >
class SPIChannel
{
public:

    SPIChannel()
    {

    }


    void Go()
    {

    }


private:

    uint8_t     outputBuffer[outputBufferSize];
    uint8_t     inputBuffer[inputBufferSize];

};


#endif


