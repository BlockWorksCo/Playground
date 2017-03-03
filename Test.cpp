


#include <stdint.h>
#include <stdio.h>
#include <tuple>
#include <utility>












int main()
{

    auto protocol = std::make_tuple(
            [](int p0)                  {printf("[One:%d]\n",p0);},
            [](int p0, int p1)          {printf("[Two:%d]\n",p0);},
            [](int p0, int p1, int p2)  {printf("[Three:%d]\n",p0);}
        );
 


    std::get<2>(protocol)(1,2,3);
}



