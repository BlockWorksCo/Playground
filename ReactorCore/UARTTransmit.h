


#ifndef __UARTTRANSMIT_H__
#define __UARTTRANSMIT_H__

template <  typename pinType, 
            typename timingType, 
            uint32_t ticksPerSecond,
            uint32_t bitsPerSecond,
            uint32_t bitsPerByte,
            uint32_t bitTolerance>
class UARTTransmit
{

public:


    UARTTransmit( pinType& _pin, timingType& _timing ) :
            pin(_pin),
            timing(_timing),
            ticksPerBit( ticksPerSecond/bitsPerSecond ),
            bitNumber(0),
            currentByte(0xab),
            previousBitTick(0)
    {
        pin.Set();
    }


    void Process()
    {
        uint32_t    deltaTick   = timing.GetTick()-previousBitTick;

        if( (deltaTick >= ticksPerBit) && (deltaTick < ticksPerBit+bitTolerance) )
        {
            bool        state   = true;

            //
            //
            //
            switch(bitNumber)
            {
                case 0:
                {
                    state   = false;    // Start bit.
                    break;
                }

                case 1:
                {
                    if( (currentByte&0x01) == 0)
                    {
                        state   = false;
                    }
                    break;
                }

                case 2:
                {
                    if( (currentByte&0x02) == 0)
                    {
                        state   = false;
                    }
                    break;
                }

                case 3:
                {
                    if( (currentByte&0x04) == 0)
                    {
                        state   = false;
                    }
                    break;
                }

                case 4:
                {
                    if( (currentByte&0x08) == 0)
                    {
                        state   = false;
                    }
                    break;
                }

                case 5:
                {
                    if( (currentByte&0x10) == 0)
                    {
                        state   = false;
                    }
                    break;
                }

                case 6:
                {
                    if( (currentByte&0x20) == 0)
                    {
                        state   = false;
                    }
                    break;
                }

                case 7:
                {
                    if( (currentByte&0x40) == 0)
                    {
                        state   = false;
                    }
                    break;
                }

                case 8:
                {
                    if( (currentByte&0x80) == 0)
                    {
                        state   = false;
                    }
                    break;
                }

                case 9:
                {
                    state   = true;     // Stop bit.
                    break;
                }

                default:
                {
                    state   = true;
                    break;
                }
            }

            //
            // Set the output state.
            //
            if(state == true)
            {
                pin.Set();            
            }
            else
            {
                pin.Clear();            
            }

            //
            // move along the bitNumber for next time.
            //            
            bitNumber   = (bitNumber + 1) % bitsPerByte;
        }
    }

private:

    pinType&    pin;
    timingType& timing;
    uint32_t    ticksPerBit;
    uint8_t     bitNumber;
    uint8_t     currentByte;
    uint32_t    previousBitTick;
};




#endif

