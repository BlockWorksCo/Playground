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
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//
//
//
template <uint8_t bitNumber>
class RPIOutput 
{
public:

    RPIOutput() :
        mask(1<<bitNumber)
    {
        //
        //
        //
        if(gpio == 0)
        {
            gpio = mapRegAddr(GPIO_BASE);
            setRegister     = gpio+GPFSET0;
            clearRegister   = gpio+GPFCLR0;
        }

        //
        //
        //
        switch(bitNumber/10)
        { 
            case 0:
                printf("GPO%d = output.\n",bitNumber);
                *(gpio + GPFSEL0) &= ~(7<<(((bitNumber)%10)*3));
                *(gpio + GPFSEL0) |=  (1<<(((bitNumber)%10)*3));
                break;
            case 1:
                printf("GPO%d = output.\n",bitNumber);
                *(gpio + GPFSEL1) &= ~(7<<(((bitNumber)%10)*3));
                *(gpio + GPFSEL1) |=  (1<<(((bitNumber)%10)*3));
                break;
            case 2:
                printf("GPO%d = output.\n",bitNumber);
                *(gpio + GPFSEL2) &= ~(7<<(((bitNumber)%10)*3));
                *(gpio + GPFSEL2) |=  (1<<(((bitNumber)%10)*3));
                break;
            default:
                break;
        }
        
    }

    
    void Toggle()
    {
    }
    
    void Set()
    {
        *setRegister = mask;
    }
    
    void Set(bool value)
    {
        if(value == false)
        {
            *clearRegister = mask;            
        }
        else
        {
            *setRegister = mask;
        }
    }
    
    void Clear()
    {
        *clearRegister = mask;
    }

private:

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
    uint32_t* mapRegAddr(unsigned long baseAddr)
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
      
       /* mmap IO */
      regAddrMap = mmap(
          NULL,             //Any adddress in our space will do
          GPIO_LEN,       //Map length
          PROT_READ|PROT_WRITE|PROT_EXEC,// Enable reading & writting to mapped memory
          MAP_SHARED|MAP_LOCKED,       //Shared with other processes
          mem_fd,           //File to map
          baseAddr         //Offset to base address
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


private:

    //gpio registers
    static const unsigned int GPFSET0 = 7;
    static const unsigned int GPFCLR0 = 10;
    static const unsigned int GPFLEV0 = 13;
    static const unsigned int GPFSEL0 = 0;
    static const unsigned int GPFSEL1 = 1;
    static const unsigned int GPFSEL2 = 2;

    static const unsigned int GPIO_BASE = 0x3f200000;// gpio registers base address
    static const unsigned int GPIO_LEN =   0xB4;// need only map B4 registers

    static volatile uint32_t*  gpio; 

    volatile uint32_t*  setRegister;
    volatile uint32_t*  clearRegister;
    uint16_t    mask;
};




//
//
//
template < uint8_t bitNumber >
class RPIInput 
{
public:

    RPIInput()
    {
        #if 0
        //
        //
        //
        switch(bitNumber/10)
        { 
            case 0:
                *(gpio + GPFSEL0) &= ~(7<<(((pinnum)%10)*3));
                break;
            case 1:
                *(gpio + GPFSEL1) &= ~(7<<(((pinnum)%10)*3));
                break;
            case 2:
                *(gpio + GPFSEL2) &= ~(7<<(((pinnum)%10)*3));
                break;
            default:
                break;
        }
        #endif
    }
    

    bool Get()
    {
        return false;
    }

private:

    //gpio registers
    static const unsigned int GPFSET0 = 7;
    static const unsigned int GPFCLR0 = 10;
    static const unsigned int GPFLEV0 = 13;
    static const unsigned int GPFSEL0 = 0;
    static const unsigned int GPFSEL1 = 1;
    static const unsigned int GPFSEL2 = 2;

    static const unsigned int GPIO_BASE = 0x3f200000;// gpio registers base address
    static const unsigned int GPIO_LEN =   0xB4;// need only map B4 registers

};




#endif






