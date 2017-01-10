





#include <stdio.h>
#include <stdint.h>




uint32_t    functionExpiryTimeStack[16]     = {0};
uint32_t    stackPtr                        = 0;
uint32_t    currentExpiryTime               = 0;

uint32_t GetCurrentTimestamp()
{
    return 0;
}

void SetTimerExpiryTime( uint32_t expiryTime )
{
    currentExpiryTime   = expiryTime;
}

template <uint32_t duration, void (*fn)() > void CallWithMaxDuration()
{
    //
    // Stack the current expiry time and setup the new expiry time.
    //
    functionExpiryTimeStack[stackPtr]     = currentExpiryTime;
    stackPtr++;
    currentExpiryTime   = GetCurrentTimestamp()+duration;

    //
    // Call the function. If the timer expires, it will PANIC.
    //
    fn();

    //
    // Unstack the old expiry time to resume timing the caller.
    //
    stackPtr--;
    SetTimerExpiryTime( functionExpiryTimeStack[stackPtr++] );
}


void SlowFunction()
{
    volatile static uint32_t   blaa    = 0;
    blaa    = 12344;
}


int main()
{
    CallWithMaxDuration<10, SlowFunction>();
}