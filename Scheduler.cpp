


#include <cstdint>
#include <stdio.h>



//
//
//
template <  uint32_t iterationDuration, 
            typename Schedulee1Type,
            typename Schedulee2Type
            >
class Scheduler
{

public:

    Scheduler(  Schedulee1Type& _schedulee1,
                Schedulee2Type& _schedulee2) :
            schedulee1(_schedulee1),
            schedulee2(_schedulee2)
    {

    }

    void Iterate( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
        schedulee1.Iterate( timestamp, inputValue, outputValue );
        schedulee2.Iterate( timestamp, inputValue, outputValue );
    }

private:

    Schedulee1Type&     schedulee1;
    Schedulee2Type&     schedulee2;

};

////////////////////////////////////////////////////////////////////////////


template <uint32_t ticksPerSecond, uint32_t ticksPerBit, uint8_t rxMask, uint32_t fifoDepth>
class UARTReceiver8N1
{
public:
    void Iterate( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
        if(timestamp > nextSampleTimestamp)
        {
            switch( sampleNumber )
            {
                case 0:
                    break;

            }

            sampleNumber            = (sampleNumber + 1) % 10;
            nextSampleTimestamp     = timestampOfStartBit + (sampleNumber * ticksPerSample);
        }
    }

    uint32_t    sampleNumber            = 0;
    uint32_t    ticksPerSample          = ticksPerBit / 4;
    uint32_t    nextSampleTimestamp     = 0;
    uint32_t    timestampOfStartBit     = 0;
};


template <uint32_t ticksPerSecond, uint32_t ticksPerBit, uint8_t txMask, uint32_t fifoDepth>
class UARTTransmitter8N1
{
public:
    void Iterate( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
        if(timestamp > nextBitTimestamp)
        {
            nextBitTimestamp    = byteStartTimestamp + (bitNumber * ticksPerBit);

            switch(bitNumber)
            {
                case 0:     // START
                    SetTxLow( outputValue );
                    break;

                case 1:     // D0
                    SetTx( currentByte & 0x01, outputValue );
                    break;

                case 2:     // D1
                    SetTx( currentByte & 0x02, outputValue );
                    break;

                case 3:     // D2
                    SetTx( currentByte & 0x04, outputValue );
                    break;

                case 4:     // D3
                    SetTx( currentByte & 0x08, outputValue );
                    break;

                case 5:     // D4
                    SetTx( currentByte & 0x10, outputValue );
                    break;

                case 6:     // D5
                    SetTx( currentByte & 0x20, outputValue );
                    break;

                case 7:     // D6
                    SetTx( currentByte & 0x40, outputValue );
                    break;

                case 8:     // D7
                    SetTx( currentByte & 0x80, outputValue );
                    break;

                case 9:     // STOP
                    SetTxHigh( outputValue );
                    currentByte     = fifo[fifoTail];
                    fifoTail++;
                    byteStartTimestamp  = timestamp;
                    break;
            }

            bitNumber           = (bitNumber+1) & 0x7;

            if( (currentByte&(1<<bitNumber)) != 0 )
            {
                outputValue     |= txMask;
            }
            else
            {
                outputValue     &= ~txMask;
            }
        }
    }

    void SetTxHigh( uint8_t& outputValue )
    {
        outputValue     |= txMask;
    }

    void SetTxLow( uint8_t& outputValue )
    {
        outputValue     &= ~txMask;
    }

    void SetTx( uint8_t zeroToClearFlag, uint8_t& outputValue )
    {
        if( zeroToClearFlag == 0 )
        {
            outputValue     &= ~txMask;
        }
        else
        {
            outputValue     |= txMask;            
        }
    }

    void PutIntoFIFO(uint8_t value)
    {
        fifo[fifoHead]  = value;
        fifoHead    = (fifoHead+1)%fifoDepth;
    }

    uint32_t    byteStartTimestamp  = 0;
    uint32_t    nextBitTimestamp    = 0;
    uint32_t    bitNumber           = 0;
    uint8_t     currentByte;

    uint32_t    fifoHead            = 0;
    uint32_t    fifoTail            = 0;
    uint8_t     fifo[fifoDepth];
};




template <uint32_t ticksPerSecond, uint32_t ticksPerBit, uint8_t txMask>
class PWM
{
public:
    void Iterate( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
        if(timestamp > nextBitTimestamp)
        {
            nextBitTimestamp    = timestamp + ticksPerBit;

            switch(bitNumber)
            {
                case 0:     // ON
                    SetTxLow( outputValue );
                    break;

                case 1:     // OFF
                    SetTxHigh( outputValue );
                    break;
            }

            bitNumber           = (bitNumber+1) & 0x2;
        }
    }

    void SetTxHigh( uint8_t& outputValue )
    {
        outputValue     |= txMask;
    }

    void SetTxLow( uint8_t& outputValue )
    {
        outputValue     &= ~txMask;
    }

    void SetTx( uint8_t zeroToClearFlag, uint8_t& outputValue )
    {
        if( zeroToClearFlag == 0 )
        {
            outputValue     &= ~txMask;
        }
        else
        {
            outputValue     |= txMask;            
        }
    }

    uint32_t    nextBitTimestamp    = 0;
    uint32_t    bitNumber           = 0;
};


////////////////////////////////////////////////////////////////////////////


//
//
//
int main()
{
    typedef UARTTransmitter8N1<100000,3, 0x01, 1024>    TxType;
    typedef UARTReceiver8N1<100000,3, 0x02, 1024>       RxType;
    TxType   one;
    RxType   two;
    Scheduler<100, TxType, RxType >  scheduler(one,two);


    while(true)
    {
        uint32_t    timestamp       = 0;
        uint8_t     outputValue     = 0;
        scheduler.Iterate( timestamp, 0xab, outputValue );

        printf("%02x\n", outputValue);

        timestamp++;
    }
}


