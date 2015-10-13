


#ifndef __UARTRECEIVE_H__
#define __UARTRECEIVE_H__

template <  typename pinType, 
            typename timingType, 
            uint32_t ticksPerSecond>
class UARTReceive
{

public:


    UARTReceive( pinType& _pin, timingType& _timing ) :
            pin(_pin),
            timing(_timing),
            state(false)
    {
        pin.Set();
    }


    void Process()
    {
        if( (timing.GetTick()%ticksPerSecond) <= 100)
        {
            if(state == true)
            {
                pin.Set();            
            }
            else
            {
                pin.Clear();            
            }            
        }
    }

private:

    pinType&    pin;
    timingType& timing;
    bool        state;

};




#endif

