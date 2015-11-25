//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __CYCLECOUNTERTIMING_H__
#define __CYCLECOUNTERTIMING_H__



//
//
//
template < typename TimestampType, uint32_t ClockRateInHz>
class Timing
{

public:

    //
    //
    //
    Timing()
    {
        // in general enable all counters (including cycle counter)
        int32_t value = 1;

        // peform reset:  
        value |= 2;     // reset all counters to zero.
        value |= 4;     // reset cycle counter to zero.

        value |= 16;

        // program the performance-counter control-register:
        //asm volatile ("MCR p15, 0, %0, c9, c12, 0\t\n" :: "r"(value));  

        // enable all counters:  
        //asm volatile ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x8000000f));  

        // clear overflows:
        //asm volatile ("MCR p15, 0, %0, c9, c12, 3\t\n" :: "r"(0x8000000f));
    }


    //
    // Return the current tick counter value.
    //
    TimestampType GetTick()
    {
        return CycleCount();
    }


    //
    // Return the current millisecond tick counter value.
    //
    TimestampType GetMicrosecondTick()
    {
        return MicrosecondCounter();
    }


    //
    // Return the current nanosecond tick counter value.
    //
    TimestampType GetNanosecondTick()
    {
        return NanosecondCounter();
    }


    //
    // Busy wait for the specified number of milliseconds.
    //
    void BusyWait(uint32_t numberOfMilliseconds)
    {
        //
        // Wait until we have a clean millisecond boundary...
        //
        TimestampType   partialPeriod   = MicrosecondCounter();
        do
        {
        } while(partialPeriod == MicrosecondCounter());

        //
        // We are now at the beginning of a period...
        // Now wait for the specified number of milliseconds.
        //
        TimestampType   endPeriod       = MicrosecondCounter()+(numberOfMilliseconds*1000);
        do
        {
        } while(MicrosecondCounter() != endPeriod);
    }

private:


    //
    //
    //        
    static inline uint32_t CycleCount()
    {
        uint32_t r = 0;
        asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
        return r;
    }


    //
    //
    //
    uint32_t NanosecondCounter()
    {
        return CycleCount()/(ClockRateInHz/100000000);
    }

    //
    //
    //
    uint32_t MicrosecondCounter()
    {
        //volatile uint32_t   cycleCount = CycleCount();
        //volatile uint32_t   usCount    = cycleCount/(ClockRateInHz/1000000);
        //return usCount;
        return CycleCount()/(ClockRateInHz/1000000);
    }

    //
    //
    //
    uint32_t MillisecondCounter()
    {
        return CycleCount()/(ClockRateInHz/1000);
    }



    //
    //
    //
};







#endif






