


#include <stdint.h>
#include <stdio.h>
#include <tuple>
#include <utility>


#define NUMBER_OF_ELEMENTS(a)   (sizeof(a)/sizeof(a[0]))


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
template< uint32_t N, uint32_t I = 0, typename... Tp>
typename std::enable_if<I == sizeof...(Tp), void>::type CallEnumerated(const std::tuple<Tp...>& t, uint32_t id)
{ 
}

template< uint32_t N, uint32_t I = 0, typename... Tp>
typename std::enable_if<I < sizeof...(Tp), void>::type CallEnumerated(const std::tuple<Tp...>& t, uint32_t id)
{
    if(std::get<I>(t).id == id)
    {
        std::get<I>(t).fn();
    }
    CallEnumerated<N,I + 1, Tp...>(t, id);        
}


typedef struct
{
    uint32_t  id;
    void      (*fn)();

} Pair;

const auto container2 = std::make_tuple(
        Pair{ .id=11,  .fn=[]()  {printf("[One:]\n");}   },
        Pair{ .id=12,  .fn=[]()  {printf("[Two:]\n");}   },
        Pair{ .id=103, .fn=[]()  {printf("[Three:]\n");} }
    );











typedef void (*fn)();

//
//
//

template<uint32_t I, uint32_t idArray[], fn fnArray[] >
typename std::enable_if<I == 0, void>::type CallEnumeratedTwo(uint32_t id)
{
    if(idArray[I] == id)
    {
        fnArray[I]();
    }
}

template<uint32_t I, uint32_t idArray[], fn fnArray[] >
typename std::enable_if<I != 0, void>::type CallEnumeratedTwo(uint32_t id)
{
    if(idArray[I] == id)
    {
        fnArray[I]();
    }
    else
    {
        CallEnumeratedTwo<I-1,idArray,fnArray>(id);        
    }
}


fn fnArray[] = 
{
    [](){printf("[One:]\n");},
    [](){printf("[Two:]\n");},
    [](){printf("[Three:]\n");},
};

uint32_t idArray[] = 
{
    11,
    12,
    103,
};




class One
{
public:
    void Demo(uint32_t id)
    {
        printf("One %d\n",id);
    }    
};

class Two
{
public:
    void Demo(uint32_t id)
    {
        printf("Two %d\n",id);
    }    
};

class Three
{
public:
    void Demo(uint32_t id)
    {
        printf("Three %d\n",id);
    }    
};

One     one;
Two     two;
Three   three;


#define ON_ID_CALL(id,obj,...)      case id:obj.CALL(__VA_ARGS__);break


auto Switcher = [](uint32_t id)
{
    #define CALL    Demo
    switch(id)
    {
        ON_ID_CALL(11,one, 999);
        ON_ID_CALL(12,two, 999);
        ON_ID_CALL(103,three, 999);
    }
};




//
//
//
int main()
{
    //CallEach( container );
    //CallIndexed<2>( container );
    //CallEnumerated<2>( container2, 12 );
    //CallEnumeratedTwo<NUMBER_OF_ELEMENTS(idArray), idArray, fnArray>( 103 );
    Switcher(103);
}



