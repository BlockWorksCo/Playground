

#include <stdint.h>
#include <stdbool.h>
#include "GPIO.h"



template <uint32_t base, uint32_t pinNumber>
class PinController
{

public:

    void Process()
    {

    }

private:


};





PinController<0x10001000, 0>    pin0;
PinController<0x10001000, 1>    pin1;
PinController<0x10001000, 2>    pin2;
PinController<0x10001000, 3>    pin3;
PinController<0x10001000, 4>    pin4;
PinController<0x10001000, 5>    pin5;
PinController<0x10001000, 6>    pin6;
PinController<0x10001000, 7>    pin7;




void RunSchedule()
{
    pin0.Process();
    pin1.Process();
    pin2.Process();
    pin3.Process();
    pin4.Process();
    pin5.Process();
    pin6.Process();
    pin7.Process();
}





int main()
{
    //
    // Forever
    //
    while(true)
    {
        //
        // Calculate the new outputs.
        //
        RunSchedule();

        //
        // Output the new values.
        //
    }

    return 0;
}


void start()
{
    main();
}