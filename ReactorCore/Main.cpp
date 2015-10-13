//
//
//

#include <stdint.h>
#include <stdbool.h>
#include "Configuration.h"




TimingType                                  timing;



//
// Raw pin definitions.
//


Pin0Type                    pin0;
Pin1Type                    pin1;
Pin2Type                    pin2;
Pin3Type                    pin3;
Pin4Type                    pin4;
Pin5Type                    pin5;
Pin6Type                    pin6;
Pin7Type                    pin7;




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



