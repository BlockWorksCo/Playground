


#include <cstdint>
#include <stdio.h>



//
//
//
template <  uint32_t iterationDuration, 
            typename Schedulee1Type,
            typename Schedulee2Type,
            typename Schedulee3Type,
            typename Schedulee4Type,
            typename Schedulee5Type,
            typename Schedulee6Type,
            typename Schedulee7Type,
            typename Schedulee8Type
            >
class Scheduler
{

public:

    Scheduler(  Schedulee1Type& _schedulee1,
                Schedulee2Type& _schedulee2,
                Schedulee3Type& _schedulee3,
                Schedulee4Type& _schedulee4,
                Schedulee5Type& _schedulee5,
                Schedulee6Type& _schedulee6,
                Schedulee7Type& _schedulee7,
                Schedulee8Type& _schedulee8
                ) :
            schedulee1(_schedulee1),
            schedulee2(_schedulee2),
            schedulee3(_schedulee3),
            schedulee4(_schedulee4),
            schedulee5(_schedulee5),
            schedulee6(_schedulee6),
            schedulee7(_schedulee7),
            schedulee8(_schedulee8)
    {

    }

    void Iterate( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
        schedulee1.Iterate( timestamp, inputValue, outputValue );
        schedulee2.Iterate( timestamp, inputValue, outputValue );
        schedulee3.Iterate( timestamp, inputValue, outputValue );
        schedulee4.Iterate( timestamp, inputValue, outputValue );
        schedulee5.Iterate( timestamp, inputValue, outputValue );
        schedulee6.Iterate( timestamp, inputValue, outputValue );
        schedulee7.Iterate( timestamp, inputValue, outputValue );
        schedulee8.Iterate( timestamp, inputValue, outputValue );
    }

private:

    Schedulee1Type&     schedulee1;
    Schedulee2Type&     schedulee2;
    Schedulee3Type&     schedulee3;
    Schedulee4Type&     schedulee4;
    Schedulee5Type&     schedulee5;
    Schedulee6Type&     schedulee6;
    Schedulee7Type&     schedulee7;
    Schedulee8Type&     schedulee8;

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
            uint8_t     state   = inputValue & rxMask;

            if(startDetected == false)
            {
                if(state == 0)
                {
                    //
                    // line has gone low, so start bit detected.
                    //
                    startDetected           = true;
                    timestampOfStartBit     = timestamp;
                    highCount               = 0;
                }
            }
            else
            {                
                //
                // Samples within the bit & byte.
                //
                uint8_t     highIncrememnt     = 0;
                if(state == 0)
                {
                    highIncrememnt     = 0;
                }   
                else
                {
                    highIncrememnt     = 1;
                }             

                //
                //
                //
                switch( sampleNumber )
                {
                    case 0:
                    case 1:
                    case 2:
                        highCount     += highIncrememnt;
                        break;

                    case 3:
                        highCount     += highIncrememnt;
                        if(highCount >= 3)
                        {
                            currentByte     = 0x01;
                        }
                        else
                        {
                            currentByte     = 0x00;
                        }
                        highCount     = 0;
                        break;

                    case 4:
                    case 5:
                    case 6:
                        highCount     += highIncrememnt;
                        break;

                    case 7:
                        highCount     += highIncrememnt;
                        if(highCount >= 3)
                        {
                            currentByte     |= 0x02;
                        }
                        highCount     = 0;
                        break;

                    case 8:
                    case 9:
                    case 10:
                        highCount     += highIncrememnt;
                        break;

                    case 11:
                        highCount     += highIncrememnt;
                        if(highCount >= 3)
                        {
                            currentByte     |= 0x04;
                        }
                        highCount     = 0;
                        break;

                }


            }

            sampleNumber            = (sampleNumber + 1) % 10;
            nextSampleTimestamp     = timestampOfStartBit + (sampleNumber * ticksPerSample);
        }
    }

    uint32_t    sampleNumber            = 0;
    uint32_t    ticksPerSample          = ticksPerBit / 4;
    uint32_t    nextSampleTimestamp     = 0;
    uint32_t    timestampOfStartBit     = 0;
    bool        startDetected           = false;
    uint8_t     previousState           = 1;
    uint8_t     highCount               = 0;
    uint8_t     currentByte             = 0;
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

            bitNumber           = (bitNumber+1) & 0x1;
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





class NoOperation
{
public:
    void Iterate( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
    }
};


////////////////////////////////////////////////////////////////////////////


//
//
//
int main()
{
    typedef UARTTransmitter8N1<100000,3, 0x01, 1024>    TxType;
    typedef UARTReceiver8N1<100000,3, 0x02, 1024>       RxType;
    typedef PWM<100000,30, 0x4>                         PWMType;
    TxType          one;
    RxType          two;
    NoOperation     nop;
    PWMType         pwm;
    Scheduler<  100, 
                TxType, 
                RxType,
                NoOperation,
                PWMType,
                NoOperation,
                NoOperation,
                NoOperation,
                NoOperation >  scheduler(one,two, nop, pwm, nop,nop, nop, nop);


    while(true)
    {
        uint32_t    timestamp       = 0;
        uint8_t     outputValue     = 0;
        scheduler.Iterate( timestamp, 0xab, outputValue );

        printf("%02x\n", outputValue);

        timestamp++;
    }
}


