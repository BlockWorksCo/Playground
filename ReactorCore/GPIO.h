//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __RPIGPIO_H__
#define __RPIGPIO_H__





#include "Platform.h" 

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//
//
//
template <uint32_t portAddress, uint8_t bitNumber>
class RPIOutput 
{
public:

    RPIOutput() :
        mask(1<<bitNumber)
    {
        gpio = mapRegAddr(GPIO_BASE);
    }

    
    void Toggle()
    {
    }
    
    void Set()
    {
    }
    
    void Set(uint16_t value)
    {
    }
    
    void Clear()
    {
    }

private:


    /*******************************************************************
     * writeGPIOReg() - Writes a 32-bit value to one of the GPIO 
     * addresses listed on lines 4-9. This function is not required for 
     * basic GPIO usage (low level access function) but is made available
     * anyways. 
     *
     * Parameters reg - Register address to write to.....see lines 4-9
     *            val - unsigned 32-bit value to write to the reg
     * Return Value - none
     * ****************************************************************/
    void writeGPIOReg(unsigned int reg, unsigned int val){
        *(this->gpio + reg) = val;
    }

    /*******************************************************************
     * readGPIOReg() - reads a 32-bit value from one of the GPIO 
     * addresses listed on lines 4-9. This function is not required for 
     * basic GPIO usage (low level access function) but is made available
     * anyways. 
     * 
     * Parameters reg - Register address to read from.....see lines 4-9
     *            val - Value of reg is written to val and passed back to
     *                  calling function/method by reference
     * Return Value - none
     * ****************************************************************/
    void readGPIOReg(unsigned int reg, unsigned int &val){
        val = *(this->gpio + reg);
        }



    /*******************************************************************
     * setPinDir() - sets the direction of a pin to either input or 
     * output
     * 
     * Parameters - pinnum - GPIO pin number as per the RPI's  BCM2835's
     *                       standard definition
     *              dir - pin direction can be INPUT for input
     *                    or OUTPUT for output
     * Return Value -None
     * *****************************************************************/
    void setPinDir(unsigned int pinnum, const unsigned int &dir){
        if (dir == OUTPUT){
            switch(pinnum/10) { 
                case 0:
                    *(this->gpio + GPFSEL0) &= ~(7<<(((pinnum)%10)*3));
                    *(this->gpio + GPFSEL0) |=  (1<<(((pinnum)%10)*3));
                    break;
                case 1:
                    *(this->gpio + GPFSEL1) &= ~(7<<(((pinnum)%10)*3));
                    *(this->gpio + GPFSEL1) |=  (1<<(((pinnum)%10)*3));
                    break;
                case 2:
                    *(this->gpio + GPFSEL2) &= ~(7<<(((pinnum)%10)*3));
                    *(this->gpio + GPFSEL2) |=  (1<<(((pinnum)%10)*3));
                    break;
                default:
                    break;
            }
        
        }
        else{
            switch(pinnum/10) { 
                case 0:
                    *(this->gpio + GPFSEL0) &= ~(7<<(((pinnum)%10)*3));
                    break;
                case 1:
                    *(this->gpio + GPFSEL1) &= ~(7<<(((pinnum)%10)*3));
                    break;
                case 2:
                    *(this->gpio + GPFSEL2) &= ~(7<<(((pinnum)%10)*3));
                    break;
                default:
                    break;
            }
        }
    }

    /*******************************************************************
     * writePinState() - sets (to 1) or clears (to 0) the state of an
     * output GPIO. This function has no effect on input GPIOs.
     * For faster output GPIO pin setting/clearing..use inline functions
     * 'writePinHigh()' & 'writePinLow()' defined in the header file 
     * 
     * Parameters - pinnum - GPIO number as per RPI and BCM2835 
     *                       standard definition
     *              pinstate - value to write to output pin...either 
     *              HIGH for setting or LOW for 
     *              clearing
     * Return Value - None           
     * ****************************************************************/
    void writePinState(unsigned int pinnum, const unsigned int &pinstate){

        if(pinstate == HIGH)
            *(this->gpio + GPFSET0) = (1 << pinnum) ;
        else    
            *(this->gpio + GPFCLR0) = (1 << pinnum);
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
    volatile unsigned *mapRegAddr(unsigned long baseAddr){
      int mem_fd = 0;
      void *regAddrMap = MAP_FAILED;

      /* open /dev/mem.....need to run program as root i.e. use sudo or su */
      if (!mem_fd) {
        if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
         perror("can't open /dev/mem");
          exit (1);
        }
      }
      
       /* mmap IO */
      regAddrMap = mmap(
          NULL,             //Any adddress in our space will do
          GPIO_LEN,       //Map length
          PROT_READ|PROT_WRITE|PROT_EXEC,// Enable reading & writting to mapped memory
          MAP_SHARED|MAP_LOCKED,       //Shared with other processes
          mem_fd,           //File to map
          baseAddr         //Offset to base address
      );
        
      if (regAddrMap == MAP_FAILED) {
          perror("mmap error");
          close(mem_fd);
          exit (1);
      }
      
      if(close(mem_fd) < 0){ //No need to keep mem_fd open after mmap
                             //i.e. we can close /dev/mem
        perror("couldn't close /dev/mem file descriptor");
        exit(1);
        }   
      return (volatile unsigned *)regAddrMap;
    }

private:
    //gpio registers
    static const unsigned int GPFSET0 = 7;
    static const unsigned int GPFCLR0 = 10;
    static const unsigned int GPFLEV0 = 13;
    static const unsigned int GPFSEL0 = 0;
    static const unsigned int GPFSEL1 = 1;
    static const unsigned int GPFSEL2 = 2;
    // two possible states for pin direction
    static const unsigned int INPUT = 0;
    static const unsigned int OUTPUT = 1;
    // two possible states for output pins
    static const unsigned int LOW = 0;
    static const unsigned int HIGH = 1;

    static const unsigned int GPIO_BASE = 0x20200000;// gpio registers base address
    static const unsigned int GPIO_LEN =   0xB4;// need only map B4 registers

    volatile unsigned int *gpio; 

    uint16_t    mask;
};




//
//
//
template <  uint32_t portAddress, 
            uint8_t bitNumber >
class RPIInput 
{
public:

    RPIInput()
    {
    }
    

    bool Get()
    {
        return false;
    }

};




#endif






