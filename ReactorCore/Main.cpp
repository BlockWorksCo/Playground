//
//
//



#include <stdint.h>
#include <stdbool.h>
#include "Configuration.h"

class GPIOMapping
{
public:

    GPIOMapping()
    {
        //
        //
        //
        printf("<mapping GPIO regs.>\n");
        gpio = mapRegAddr();
    }


    /********************************************************************
     *  volatile unsigned *mapRegAddr(unsigned long baseAddr)
     * This function maps a block of physical memory into the memory of 
     * the calling process. It enables a user space process to access 
     * registers in physical memory directly without having to interact 
     * with in kernel side code i.e. device drivers
     *
     * Parameter - baseAddr (unsigned long) - this is the base address of
     * a block of physical memory that will be mapped into the userspace 
     * process memory. 
     *******************************************************************/ 
    uint32_t* mapRegAddr()
    {
      int mem_fd = 0;
      void *regAddrMap = MAP_FAILED;

      /* open /dev/mem.....need to run program as root i.e. use sudo or su */
      if (!mem_fd) 
      {
        if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
        {
         perror("can't open /dev/mem");
          exit (1);
        }
      }
        static const unsigned int GPIO_BASE = 0x3f200000;// gpio registers base address
        static const unsigned int GPIO_LEN =   0xB4;// need only map B4 registers
      
       /* mmap IO */
      regAddrMap = mmap(
          NULL,             //Any adddress in our space will do
          GPIO_LEN,       //Map length
          PROT_READ|PROT_WRITE|PROT_EXEC,// Enable reading & writting to mapped memory
          MAP_SHARED|MAP_LOCKED,       //Shared with other processes
          mem_fd,           //File to map
          GPIO_BASE         //Offset to base address
      );
        
      if (regAddrMap == MAP_FAILED) 
      {
          perror("mmap error");
          close(mem_fd);
          exit (1);
      }
      
      if(close(mem_fd) < 0)
      { //No need to keep mem_fd open after mmap
                             //i.e. we can close /dev/mem
        perror("couldn't close /dev/mem file descriptor");
        exit(1);
        }   

      printf("GPIO mapped to %08x\n",(uint32_t)regAddrMap);
      return (uint32_t*)regAddrMap;
    }

};




GPIOMapping                 gpioMapping;
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

RPIOutput<4>  debugPin;


volatile uint32_t* gpio    = 0;


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






void TimingCalibration()
{

    while(true)
    {
        uint32_t            currentTicks        = timing.GetTick();
        static uint32_t     ticksAtLastClock    = 0;
        uint32_t            deltaTicks          = currentTicks - ticksAtLastClock;

        if( deltaTicks >= (TICKS_PER_SECOND/10000000) )
        {
            //printf("tick...\n");
            debugPin.Toggle();
            ticksAtLastClock     = currentTicks;
        }        
    }

}




//
//
//
int main()
{
    printf("ReactorCore 0.1\n");

    TimingCalibration();

    //
    //
    //
    while(true)
    {
        debugPin.Set();
        debugPin.Clear();        
        for(volatile uint32_t i=0; i<0x20; i++);
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


