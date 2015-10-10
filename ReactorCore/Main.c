

#include <stdint.h>
#include <stdbool.h>


uint8_t     inputs  = 0x00;



void Pin0()
{
    return true;
}

void Pin1()
{
    return true;
}

void Pin2()
{
    return true;
}

void Pin3()
{
    return true;
}

void Pin4()
{
    return true;
}

void Pin5()
{
    return true;
}

void Pin6()
{
    return true;
}

void Pin7()
{
    return true;    
}


void RunSchedule()
{
    outputs     = Pin0();
    outputs     |= Pin1() << 1;
    outputs     |= Pin2() << 2;
    outputs     |= Pin3() << 3;
    outputs     |= Pin4() << 4;
    outputs     |= Pin5() << 5;
    outputs     |= Pin6() << 6;
    outputs     |= Pin7() << 7;
}





int main()
{
    //
    // Forever
    //
    while(true)
    {
        //
        // Get the inputs.
        //
        inputs  = 0xab;

        //
        // Calculate the new outputs.
        //
        outputs     = RunSchedule();

        //
        // Output the new values.
        //
    }

    return 0;
}

