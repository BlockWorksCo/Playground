


#include <cstdint>
#include <stdio.h>
#include <stdint.h>


typedef uint8_t     uint8x8_t[8];



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

    void PeriodicProcessing( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
        static uint8x8_t    bits;
        static uint8x8_t    previousBits;

#define PROCESS_SCHEDULEE(bitNumber, s)                                         \
        bits[bitNumber]     = (inputValue & (1<<bitNumber)) >> bitNumber;       \
        if( (bits[bitNumber] == 0) && (previousBits[bitNumber == 1]) )          \
        {                                                                       \
            s.ProcessNegativeEdge(timestamp);                                   \
        }                                                                       \
        if( (bits[bitNumber] == 1) && (previousBits[bitNumber == 0]) )          \
        {                                                                       \
            s.ProcessPositiveEdge(timestamp);                                   \
        }                                                                       \
        if( (timestamp%s.GetPeriod()) == 0)                                     \
        {                                                                       \
            s.PeriodicProcessing( timestamp, inputValue, outputValue );         \
        }                                                                       \
        previousBits[bitNumber]     = bits[bitNumber];

        PROCESS_SCHEDULEE(0, schedulee1);
        PROCESS_SCHEDULEE(1, schedulee2);
        PROCESS_SCHEDULEE(2, schedulee3);
        PROCESS_SCHEDULEE(3, schedulee4);
        PROCESS_SCHEDULEE(4, schedulee5);
        PROCESS_SCHEDULEE(5, schedulee6);
        PROCESS_SCHEDULEE(6, schedulee7);
        PROCESS_SCHEDULEE(7, schedulee8);
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


template <uint32_t period, uint32_t ticksPerBit, uint8_t rxMask, uint32_t fifoDepth>
class UARTReceiver8N1
{
public:

    uint32_t GetPeriod()
    {
        return period;
    }

    void ProcessNegativeEdge( uint32_t timestamp )
    {
        if(startDetected == false)
        {
            //
            // Start of byte.
            //
            startDetected       = true;
            timestampOfStartBit = timestamp;
            bitNumber 	        = 0;
            currentByte         = 0;
        }
        else
        {
            //
            // Within byte.
            //
        }

        currentLevel    = 0;
    }

    void ProcessPositiveEdge( uint32_t timestamp )
    {
        currentLevel    = 1;
    }


    void PeriodicProcessing( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
        currentByte <<= 1;
        currentByte |= currentLevel;

        bitNumber++;
        if(bitNumber >= 10)
        {
            startDetected 	= false;
        }
    }

    uint32_t    timestampOfStartBit     = 0;
    bool        startDetected           = false;
    uint8_t     currentByte             = 0;
    uint8_t     currentLevel            = 0;
    uint8_t     bitNumber               = 0;
};


template <uint32_t period, uint32_t ticksPerBit, uint8_t txMask, uint32_t fifoDepth>
class UARTTransmitter8N1
{
public:

    uint32_t GetPeriod()
    {
        return period;
    }

    void ProcessNegativeEdge( uint32_t timestamp )
    {
    }

    void ProcessPositiveEdge( uint32_t timestamp )
    {
    }



    void PeriodicProcessing( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
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




template <uint32_t period, uint32_t ticksPerBit, uint8_t txMask>
class PWM
{
public:

    uint32_t GetPeriod()
    {
        return period;
    }

    void ProcessNegativeEdge( uint32_t timestamp )
    {
    }

    void ProcessPositiveEdge( uint32_t timestamp )
    {
    }


    void PeriodicProcessing( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
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

    uint32_t GetPeriod()
    {
        return 0xffffffff;
    }

    void ProcessNegativeEdge( uint32_t timestamp )
    {
    }

    void ProcessPositiveEdge( uint32_t timestamp )
    {
    }

    void PeriodicProcessing( uint32_t timestamp, uint8_t inputValue, uint8_t& outputValue )
    {
    }
};


////////////////////////////////////////////////////////////////////////////


//
//
//
int main()
{
    typedef UARTTransmitter8N1<10,3, 0x01, 1024>    TxType;
    typedef UARTReceiver8N1<8,3, 0x02, 1024>        RxType;
    typedef PWM<30,30, 0x4>                         PWMType;
    TxType          one;
    RxType          two;
    NoOperation     nop;
    PWMType         pwm;
    Scheduler<  100, 
                RxType, 
                RxType,
                RxType,
                RxType,
                RxType,
                RxType,
                RxType,
                RxType >  scheduler(two,two, two, two, two,two, two, two);


    uint8x8_t   bits            = {0};
    uint8x8_t   previousBits    = {0};

    for(uint32_t i=0; i<250000000; i++)
    {
        static uint8_t 	inputValues[]	= {0x08, 0x40,0x41,0x48,0x04,0x81,0x08,0x14,0x18,0x01,0x81,0x80,0x80,0x18,0x40,0x04,0x18,0x04,0x00,0x40};
        uint32_t 	inputIndex 	= 0;
        uint32_t    timestamp       = 0;
        uint8_t     outputValue     = 0;
        uint8_t     inputValue      = inputValues[inputIndex];

        scheduler.PeriodicProcessing( timestamp, 0xab, outputValue );
        static volatile uint8_t 	r = outputValue;

        timestamp++;
    inputIndex++;
    }
}


