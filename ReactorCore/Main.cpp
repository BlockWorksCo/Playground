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




extern unsigned long __preinit_array_start;
extern unsigned long __preinit_array_end;
extern unsigned long __init_array_start;
extern unsigned long __init_array_end;
extern unsigned long _ctor_start;
extern unsigned long _ctor_end;
static void call_constructors(unsigned long *start, unsigned long *end) __attribute__((noinline));
static void call_constructors(unsigned long *start, unsigned long *end)
{
  unsigned long *i;
  void (*funcptr)();
  for ( i = start; i < end; i++)
  {
    funcptr=(void (*)())(*i);
    funcptr();
  }
}




extern "C" void __cxa_pure_virtual() 
{ 
    while (1); 
}



extern "C" void abort(void)
{
    while(true);
}

//
//
//
int main()
{
    SystemInit();

    //
    //Call C++ global constructors
    //
    call_constructors(&__preinit_array_start, &__preinit_array_end);
    call_constructors(&__init_array_start, &__init_array_end);


    volatile uint32_t    timestamp1  = timing.GetTick();
    volatile uint32_t    timestamp2  = timing.GetTick();
    volatile uint32_t    timestamp3  = timing.GetTick();

    while(true)
    {
        volatile uint32_t    timestamp     = timing.GetTick();
        volatile uint32_t    t   = timestamp % 168;

        if( t < 40)
        {
            GPIOD->ODR  = ~GPIOD->ODR;
        }
        //pin0.Set();
        //pin0.Clear();
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
        RunSchedule();            

        //
        // Data transfer.
        //
        transferController.Go();
    }

    return 0;
}



extern "C" void WWDG_IRQHandler(void)
{
    while(true);
}