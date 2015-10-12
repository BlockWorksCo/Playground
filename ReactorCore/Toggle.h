


#ifndef __TOGGLE_H__
#define __TOGGLE_H__

template <  typename pinType, 
            typename timingType, 
            uint32_t ticksPerSecond>
class ToggleController
{

public:


    ToggleController( pinType& _pin, timingType& _timing ) :
            pin(_pin),
            timing(_timing),
            state(false)
    {
        pin.Set();
    }


    void Process()
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

private:

    pinType&    pin;
    timingType& timing;
    bool        state;

};




#endif

