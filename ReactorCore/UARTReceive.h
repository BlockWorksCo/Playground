


#ifndef __UARTRECEIVE_H__
#define __UARTRECEIVE_H__

template <  typename pinType, 
            typename timingType, 
            uint32_t ticksPerSecond,
            uint32_t bitsPerSecond,
            uint32_t bitsPerByte,
            uint32_t sampleTolerance,
            uint32_t samplesPerBit,
            typename transferType >
class UARTReceive
{

public:


    UARTReceive( pinType& _pin, timingType& _timing, transferType& _transferController ) :
            pin(_pin),
            timing(_timing),
            ticksPerBit( ticksPerSecond/bitsPerSecond ),
            bitNumber(0),
            currentByte(0xab),
            previousSampleTick(0),
            ticksPerSample( ticksPerBit / samplesPerBit ),
            sampleNumber(0),
            transferController(_transferController)
    {
    }


    void Process()
    {
        uint32_t    currentTick = timing.GetTick();
        uint32_t    deltaTick   = currentTick-previousSampleTick;

        if( (deltaTick >= ticksPerSample) && (deltaTick < (ticksPerSample+sampleTolerance)) )
        {

            //
            // Sample the input line.
            //
            bool        sample      = pin.Get();

            //
            // Reset the counts on sample 0.
            //
            if(sampleNumber == 0)
            {
                highSampleCount     = 0;
                lowSampleCount      = 0;
            }

            //
            // Update the counts.
            //
            if(sample == true)
            {
                highSampleCount++;
            }
            else
            {
                lowSampleCount++;
            }

            //
            // Last sample in this bit, so lets process the bit.
            //
            if(sampleNumber == (samplesPerBit-1) )
            {
                bool    bitState    = false;
                if(highSampleCount > lowSampleCount)
                {
                    bitState    = true;
                }

                ProcessBit( bitState );
            }

            //
            // move along the bitNumber for next time.
            //            
            sampleNumber        = (sampleNumber + 1) % samplesPerBit;
            previousSampleTick  = currentTick;
        }
    }

private:


    void ProcessBit( bool bit )
    {
        //
        // Is it the start bit.
        //
        if(bitNumber == 0)
        {
            currentStartBit     = bit;
        }

        //
        // Data bit?
        //
        if( (bitNumber >= 1) && (bitNumber < 9) )
        {
            if(bit == true)
            {
                currentByte     |= (0x01 << (bitNumber-1));                
            }
            else
            {
                currentByte     &= ~(0x01 << (bitNumber-1));                                
            }
        }

        //
        // Is it the stop bit.
        //
        if( bitNumber == bitsPerByte-1 )
        {
            currentStopBit      = bit;

            if( (currentStartBit == false) && (currentStopBit == true) )
            {
                //
                // ...byte received.
                //
                //printf("byte received %02x\n", currentByte);
            }
        }


        bitNumber   = (bitNumber + 1) % bitsPerByte;
    }


    pinType&        pin;
    timingType&     timing;
    uint32_t        ticksPerBit;
    uint8_t         bitNumber;
    uint8_t         currentByte;
    uint32_t        previousSampleTick;
    uint8_t         ticksPerSample;
    uint8_t         sampleNumber;
    uint8_t         highSampleCount;
    uint8_t         lowSampleCount;
    bool            currentStartBit;
    bool            currentStopBit;
    transferType&   transferController;

};




#endif

