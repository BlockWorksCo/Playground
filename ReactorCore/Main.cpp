//
//
//



#include <stdint.h>
#include <stdbool.h>
#include "Configuration.h"



//
// see linux/arch/mac-bcm2709/include/mach/platform.h
//
#define PERIPHERAL_BASE         (0x3F000000UL)
//#define PERIPHERAL_BASE         (0x20000000UL)
#define GPIO_BASE               (PERIPHERAL_BASE+0x00200000)
#define INTERRUPT_BASE          (PERIPHERAL_BASE+0xB000)



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

        if( deltaTicks >= (TICKS_PER_SECOND/1000000) )
        {
            //printf("tick...\n");
            debugPin.Toggle();
            ticksAtLastClock     = currentTicks;
        }        
    }

}







volatile unsigned* intrupt;

void interrupts(int flag)
  {
  static unsigned int sav132 = 0;
  static unsigned int sav133 = 0;
  static unsigned int sav134 = 0;

  if(flag == 0)    // disable
    {
    if(sav132 != 0)
      {
      // Interrupts already disabled so avoid printf
      return;
      }

    if( (*(intrupt+128) | *(intrupt+129) | *(intrupt+130)) != 0)
      {
      printf("Pending interrupts\n");  // may be OK but probably
      return;                       // better to wait for the
      }                                // pending interrupts to
                                       // clear

    sav134 = *(intrupt+134);
    *(intrupt+137) = sav134;
    sav132 = *(intrupt+132);  // save current interrupts
    *(intrupt+135) = sav132;  // disable active interrupts
    sav133 = *(intrupt+133);
    *(intrupt+136) = sav133;
    }
  else            // flag = 1 enable
    {
    if(sav132 == 0)
      {
      printf("Interrupts not disabled\n");
      return;
      }

    *(intrupt+132) = sav132;    // restore saved interrupts
    *(intrupt+133) = sav133;
    *(intrupt+134) = sav134;
    sav132 = 0;                 // indicates interrupts enabled
    }
}


void SetupInterrupts()
{
    void*   int_map;

    int memfd = open("/dev/mem",O_RDWR|O_SYNC);
    if(memfd < 0)
    {
        printf("Mem open error\n");
        return;
    }

    int_map = mmap(NULL,4096,PROT_READ|PROT_WRITE, MAP_SHARED,memfd,INTERRUPT_BASE);    

    // interrupt pointer
    intrupt = (volatile unsigned *)int_map;
}


/* enable IRQ interrupts */
void enable_interrupts (void)
{
    unsigned long temp;
    __asm__ __volatile__("mrs %0, cpsr\n"
                 "bic %0, %0, #0x80\n"
                 "msr cpsr_c, %0"
                 : "=r" (temp)
                 :
                 : "memory");
}
/*
 * disable IRQ/FIQ interrupts
 * returns true if interrupts had been enabled before we disabled them
 */
int disable_interrupts (void)
{
    unsigned long old,temp;
    __asm__ __volatile__("mrs %0, cpsr\n"
                 "orr %1, %0, #0xc0\n"
                 "msr cpsr_c, %1"
                 : "=r" (old), "=r" (temp)
                 :
                 : "memory");
    return (old & 0x80) == 0;
}



//
//
//
int main()
{
    printf("ReactorCore 0.1\n");

    //
    //
    //
    SetupInterrupts();
    //interrupts(0);
    //*(intrupt+(0x21C/4))    = 0xffffffff;
    //*(intrupt+(0x220/4))    = 0xffffffff;
    //*(intrupt+(0x224/4))    = 0xffffffff;
    //disable_interrupts();

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


