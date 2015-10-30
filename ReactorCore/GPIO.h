//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __RPIGPIO_H__
#define __RPIGPIO_H__





#include "Platform.h" 



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






