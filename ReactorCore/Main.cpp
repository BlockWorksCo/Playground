//
//
//

#include <stdint.h>
#include <stdbool.h>
#include "GPIO.h"
#include "CycleCounterTiming.h"



//
// Timing related definitions.
//
const uint32_t  CLOCK_RATE              = 200000000;
typedef Timing<uint32_t, CLOCK_RATE>        TimingType;
TimingType                                  timing;



//
// Raw pin definitions.
//
typedef STM32Output<GPIOD_BASE, 0>            Pin0Type;
typedef STM32Output<GPIOD_BASE, 1>            Pin1Type;
typedef STM32Output<GPIOD_BASE, 2>            Pin2Type;
typedef STM32Output<GPIOD_BASE, 3>            Pin3Type;
typedef STM32Output<GPIOD_BASE, 4>            Pin4Type;
typedef STM32Output<GPIOD_BASE, 5>            Pin5Type;
typedef STM32Output<GPIOD_BASE, 6>            Pin6Type;
typedef STM32Output<GPIOD_BASE, 7>            Pin7Type;

Pin0Type                    pin0;
Pin1Type                    pin1;
Pin2Type                    pin2;
Pin3Type                    pin3;
Pin4Type                    pin4;
Pin5Type                    pin5;
Pin6Type                    pin6;
Pin7Type                    pin7;

//
// User configuration.
//
#include "Configuration.h"




//
//
//
void RunSchedule()
{
    pin0Controller.Process();
    pin1Controller.Process();
    pin2Controller.Process();
    pin3Controller.Process();
    pin4Controller.Process();
    pin5Controller.Process();
    pin6Controller.Process();
    pin7Controller.Process();
}


//
// Rx and Tx data from/to the SPI port.
//
void TransferUserData()
{

}



//
//
//
int main()
{
    //
    // Forever
    //
    while(true)
    {
        uint32_t    previousTimestamp       = 0;
        uint32_t    nanosecondTimestamp     = timing.GetNanosecondTick();

        //
        // Calculate the new outputs.
        //
        if(nanosecondTimestamp !=  previousTimestamp)
        {
            RunSchedule();            
        }

        //
        // Data transfer.
        //
        TransferUserData();

        //
        //
        //
        previousTimestamp   = nanosecondTimestamp;
    }

    return 0;
}



