


#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <memory.h>

template <  typename pinType, 
            typename timingType, 
            uint32_t ticksPerSecond,
            uint32_t clocksPerSecond,
            uint32_t tolerance,
            typename transferType>
class Clock
{

public:


    Clock( pinType& _pin, timingType& _timing, transferType& _transferController ) :
            pin(_pin),
            timing(_timing),
            state(false),
            ticksPerClock( ticksPerSecond / clocksPerSecond ),
            ticksAtLastClock(0),
            transferController(_transferController)
    {
        pin.Set();
    }


    void Process()
    {
        uint32_t    currentTicks    = timing.GetTick();
        uint32_t    deltaTicks      = currentTicks - ticksAtLastClock;

        if( deltaTicks >= ticksPerClock )
        {
            pin.Toggle();

            ticksAtLastClock    = currentTicks;
        }
    }

private:

    pinType&            pin;
    timingType&         timing;
    bool                state;
    uint32_t            ticksPerClock;
    uint32_t            ticksAtLastClock;
    transferType        transferController;

};




#endif

