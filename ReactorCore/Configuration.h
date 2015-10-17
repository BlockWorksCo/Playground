


#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__


#include "GPIO.h"
#include "CycleCounterTiming.h"

#include "Toggle.h"
#include "UARTTransmit.h"
#include "UARTReceive.h"
#include "Clock.h"
#include "SPIChannel.h"


//
//
//
typedef SPIChannel<0x1, 1024, 1024>         TransferChannelType;
extern TransferChannelType                      transferChannel;



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



ToggleController<Pin0Type, TimingType, CLOCK_RATE, TransferChannelType>              pin0Controller(pin0, timing, transferChannel);
ToggleController<Pin1Type, TimingType, CLOCK_RATE, TransferChannelType>              pin1Controller(pin1, timing, transferChannel);
UARTReceive<Pin2Type, TimingType, CLOCK_RATE, 9600,8,10,8, TransferChannelType>      pin2Controller(pin2, timing, transferChannel);
UARTTransmit<Pin3Type, TimingType, CLOCK_RATE, 9600,8,100, TransferChannelType>      pin3Controller(pin3, timing, transferChannel);
ToggleController<Pin4Type, TimingType, CLOCK_RATE, TransferChannelType>              pin4Controller(pin4, timing, transferChannel);
ToggleController<Pin5Type, TimingType, CLOCK_RATE, TransferChannelType>              pin5Controller(pin5, timing, transferChannel);
Clock<Pin7Type, TimingType, CLOCK_RATE, 1000, 100, TransferChannelType>              pin7Controller(pin7, timing, transferChannel);
Clock<Pin6Type, TimingType, CLOCK_RATE, 2000, 100, TransferChannelType>              pin6Controller(pin6, timing, transferChannel);



#endif




