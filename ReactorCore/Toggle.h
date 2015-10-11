


#ifndef __TOGGLE_H__
#define __TOGGLE_H__

template <typename pinType>
class ToggleController
{

public:


    ToggleController( pinType& _pin ) :
            pin(_pin),
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
    bool        state;

};




#endif

