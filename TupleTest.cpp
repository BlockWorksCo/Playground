


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
template< uint32_t N, uint32_t I = 0, typename... Tp>
typename std::enable_if<I == sizeof...(Tp), void>::type CallEnumerated(std::tuple<Tp...>& t, uint32_t id)
{ 
}

template< uint32_t N, uint32_t I = 0, typename... Tp>
typename std::enable_if<I < sizeof...(Tp), void>::type CallEnumerated(std::tuple<Tp...>& t, uint32_t id)
{
    if(std::get<I>(t).id == id)
    {
        std::get<I>(t).fn();
    }
    CallEnumerated<N,I + 1, Tp...>(t, id);        
}









//
//
//
auto container = std::make_tuple(
        []()  {printf("[One:]\n");},
        []()  {printf("[Two:]\n");},
        []()  {printf("[Three:]\n");}
    );


typedef struct
{
    uint32_t id;
    void     (*fn)();

} Pair;

auto container2 = std::make_tuple(
        Pair{ .id=11,  .fn=[]()  {printf("[One:]\n");}   },
        Pair{ .id=12,  .fn=[]()  {printf("[Two:]\n");}   },
        Pair{ .id=103, .fn=[]()  {printf("[Three:]\n");} }
    );


//
//
//
int main()
{


    //CallEach( container );

    //CallIndexed<2>( container );
    CallEnumerated<2>( container2, 103 );
}



