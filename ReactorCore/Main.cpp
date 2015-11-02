//
//
//



#include <stdint.h>
#include <stdbool.h>
#include "Configuration.h"



TransferChannelType                             transferController;


TimingType                                  timing;



//
// Raw pin definitions.
//

/*
Pin0Type                    pin0;
Pin1Type                    pin1;
Pin2Type                    pin2;
Pin3Type                    pin3;
Pin4Type                    pin4;
Pin5Type                    pin5;
Pin6Type                    pin6;
Pin7Type                    pin7;
*/
RPIOutput<4>  debugPin;


template<uint8_t bitNumber> volatile uint32_t* RPIOutput<bitNumber>::gpio    = 0;
/*

//
//
//
void RunSchedule()
{
    pin0Controller.Process();       // 60ns
    pin1Controller.Process();       // 60ns
    pin2Controller.Process();       // 130ns
debugPin.Set();
    pin3Controller.Process();       // 160ns
debugPin.Clear();
    pin4Controller.Process();       // 130ns
    pin5Controller.Process();       // 60ns
    pin6Controller.Process();     // 130ns
    pin7Controller.Process();       // 130ns
}


*/




void TimingCalibration()
{

    while(true)
    {
        //uint32_t            currentTicks        = timing.GetTick();
        //static uint32_t     ticksAtLastClock    = 0;
        //uint32_t            deltaTicks          = currentTicks - ticksAtLastClock;

        //if( deltaTicks >= TICKS_PER_SECOND )
        {
           //debugPin.Toggle();
            debugPin.Set();
            debugPin.Clear();
            //ticksAtLastClock     = currentTicks;
        }        
    }

}




//
//
//
int main()
{
    while(true)
    {
        debugPin.Set();
        debugPin.Clear();        
    }

    //
    // Forever
    //
    while(true)
    {
        //
        // Calculate the new outputs.
        // Note: The PinControllers are idempotent, so can be applied repeatedly.
        //
        //RunSchedule();            

        //
        // Data transfer.
        //
        transferController.Go();
    }

    return 0;
}


