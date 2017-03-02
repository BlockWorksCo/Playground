


#include <cstdint>




//
//
//
template <  uint32_t iterationDuration, 
            typename Schedulee1Type,
            typename Schedulee2Type
            >
class Scheduler
{

public:

    Scheduler(  Schedulee1Type& _schedulee1,
                Schedulee2Type& _schedulee2) :
            schedulee1(_schedulee1),
            schedulee2(_schedulee2)
    {

    }

    void Iterate( )
    {

    }

private:

    Schedulee1Type&     schedulee1;
    Schedulee2Type&     schedulee2;

};




//
//
//
int main()
{
    uint32_t    one;
    uint32_t    two;
    Scheduler<100, uint32_t, uint32_t>  scheduler(one,two);


    while(true)
    {
        scheduler.Iterate();
    }
}


