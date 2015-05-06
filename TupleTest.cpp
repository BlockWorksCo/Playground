


#include <stdint.h>
#include <stdio.h>
#include <tuple>
#include <utility>


//
//
//
template<std::size_t I = 0, typename... Tp>
typename std::enable_if<I == sizeof...(Tp), void>::type CallEach(std::tuple<Tp...>& t)
{ 
}

template<std::size_t I = 0, typename... Tp>
typename std::enable_if<I < sizeof...(Tp), void>::type CallEach(std::tuple<Tp...>& t)
{
    std::get<I>(t)();
    CallEach<I + 1, Tp...>(t);
}



//
//
//
int main()
{

    auto container = std::make_tuple(
            []()  {printf("[One:]\n");},
            []()  {printf("[Two:]\n");},
            []()  {printf("[Three:]\n");}
        );
 

    CallEach( container );
}



