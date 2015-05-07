


#include <stdint.h>
#include <stdio.h>
#include <tuple>
#include <utility>


//
//
//
template< uint32_t I = 0, typename... Tp>
typename std::enable_if<I == sizeof...(Tp), void>::type CallEach(std::tuple<Tp...>& t)
{ 
}

template< uint32_t I = 0, typename... Tp>
typename std::enable_if<I < sizeof...(Tp), void>::type CallEach(std::tuple<Tp...>& t)
{
    std::get<I>(t)();
    CallEach<I + 1, Tp...>(t);
}



//
//
//
template< uint32_t N, uint32_t I = 0, typename... Tp>
typename std::enable_if<I == sizeof...(Tp), void>::type CallIndexed(std::tuple<Tp...>& t)
{ 
}

template< uint32_t N, uint32_t I = 0, typename... Tp>
typename std::enable_if<I < sizeof...(Tp), void>::type CallIndexed(std::tuple<Tp...>& t)
{
    if(I == N)
    {
        std::get<I>(t)();
    }
    CallIndexed<N,I + 1, Tp...>(t);        
}









//
//
//
auto container = std::make_tuple(
        []()  {printf("[One:]\n");},
        []()  {printf("[Two:]\n");},
        []()  {printf("[Three:]\n");}
    );


//
//
//
int main()
{


    //CallEach( container );

    CallIndexed<2>( container );
}



