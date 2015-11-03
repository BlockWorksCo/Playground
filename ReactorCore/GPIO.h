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

extern volatile uint32_t*  gpio; 


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
            perror("<NO GPIO mapping!>\n");
        }
        else
        {
            printf("GPIO mapping ok.\n");
        }

        //
        //
        //
        setRegister     = gpio+GPFSET0;
        clearRegister   = gpio+GPFCLR0;

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

    //gpio registers
    static const unsigned int GPFSET0 = 7;
    static const unsigned int GPFCLR0 = 10;
    static const unsigned int GPFLEV0 = 13;
    static const unsigned int GPFSEL0 = 0;
    static const unsigned int GPFSEL1 = 1;
    static const unsigned int GPFSEL2 = 2;

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
};




#endif






