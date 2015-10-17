


#ifndef __TOGGLE_H__
#define __TOGGLE_H__

template <  typename pinType, 
            typename timingType, 
            uint32_t ticksPerSecond,
            typename transferType>
class ToggleController
{

public:


    ToggleController( pinType& _pin, timingType& _timing, transferType& _transferController ) :
            pin(_pin),
            timing(_timing),
            state(false),
            transferController(_transferController)
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

    pinType&        pin;
    timingType&     timing;
    bool            state;
    transferType&   transferController;

};




#endif

