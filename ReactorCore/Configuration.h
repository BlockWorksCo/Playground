


#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__


#include "GPIO.h"
#include "CycleCounterTiming.h"

#include "Toggle.h"
#include "UARTTransmit.h"
#include "UARTReceive.h"
#include "Clock.h"
#include "SPIChannel.h"



const uint32_t  TICKS_PER_SECOND        = (168*1000*1000)+1;


//
//
//
typedef SPIChannel<0x1, 1024, 1024>         TransferChannelType;
extern TransferChannelType                      transferController;



//
// Timing related definitions.
//
const uint32_t  CLOCK_RATE              = 200000000;
typedef Timing<uint32_t, CLOCK_RATE>        TimingType;


typedef STM32Output<GPIOD_BASE, 0>            Pin0Type;
typedef STM32Output<GPIOD_BASE, 1>            Pin1Type;
typedef STM32Input<GPIOD_BASE, 2>             Pin2Type;
typedef STM32Output<GPIOD_BASE, 3>            Pin3Type;
typedef STM32Output<GPIOD_BASE, 4>            Pin4Type;
typedef STM32Output<GPIOD_BASE, 5>            Pin5Type;
typedef STM32Output<GPIOD_BASE, 6>            Pin6Type;
typedef STM32Output<GPIOD_BASE, 7>            Pin7Type;



extern TimingType                                  timing;
extern Pin0Type                    pin0;
extern Pin1Type                    pin1;
extern Pin2Type                    pin2;
extern Pin3Type                    pin3;
extern Pin4Type                    pin4;
extern Pin5Type                    pin5;
extern Pin6Type                    pin6;
extern Pin7Type                    pin7;

extern STM32Output<GPIOE_BASE, 0>  debugPin;



ToggleController<Pin0Type, TimingType, TICKS_PER_SECOND, TransferChannelType>           pin0Controller(pin0, timing, transferController);
ToggleController<Pin1Type, TimingType, TICKS_PER_SECOND, TransferChannelType>           pin1Controller(pin1, timing, transferController);
UARTReceive<Pin2Type, TimingType, TICKS_PER_SECOND, 115200,8,10,8, TransferChannelType>   pin2Controller(pin2, timing, transferController);
ToggleController<Pin3Type, TimingType, TICKS_PER_SECOND, TransferChannelType>           pin3Controller(pin3, timing, transferController);
Clock<Pin4Type, TimingType, TICKS_PER_SECOND, 1000, 1, TransferChannelType>             pin4Controller(pin4, timing, transferController);
Clock<Pin5Type, TimingType, TICKS_PER_SECOND, 2000, 100, TransferChannelType>           pin5Controller(pin5, timing, transferController);
UARTTransmit<Pin6Type, TimingType, TICKS_PER_SECOND, 115200,8,100, TransferChannelType>   pin6Controller(pin6, timing, transferController);
Clock<Pin7Type, TimingType, TICKS_PER_SECOND, 1000, 100, TransferChannelType>           pin7Controller(pin7, timing, transferController);



#endif




