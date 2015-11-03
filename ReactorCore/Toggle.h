


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
            transferController(_transferController)
    {
        //pin.Set();
    }


    void Process()
    {
        pin.Toggle();
    }

private:

    pinType&        pin;
    timingType&     timing;
    transferType&   transferController;

};




#endif

