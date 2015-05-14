


#include <stdint.h>
#include <stdio.h>
#include <tuple>
#include <utility>


#define NUMBER_OF_ELEMENTS(a)   (sizeof(a)/sizeof(a[0]))

#if 0

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


#endif

class One
{
public:
    void Demo(uint32_t id)
    {
        printf("One %d\n",id);
    }    

    void DoThis()
    {
        printf("This1\n");
    }    

    void DoThat()
    {
        printf("That1\n");
    }    
};

class Two
{
public:
    void Demo(uint32_t id)
    {
        printf("Two %d\n",id);
    }    

    void DoThis()
    {
        printf("This2\n");
    }    

    void DoThat()
    {
        printf("That2\n");
    }    
};

class Three
{
public:
    void Demo(uint32_t id)
    {
        printf("Three %d\n",id);
    }    

    void DoThis()
    {
        printf("This3\n");
    }    

    void DoThat()
    {
        printf("That3\n");
    }    
};




One     one;
Two     two;
Three   three;


#if 0


void DoSomething( void (*fn)(uint32_t) )
{
    fn( 11 );
    fn( 12 );
    fn( 103 );
}






#define ON_ID_CALL_METHOD(id,obj,...)      case id:obj.METHOD(__VA_ARGS__);break


auto Switcher = [](uint32_t id)
{
    #define METHOD    Demo
    switch(id)
    {
        ON_ID_CALL_METHOD(11,  one,    id);
        ON_ID_CALL_METHOD(12,  two,    id);
        ON_ID_CALL_METHOD(103, three,  id);
    }
};

#endif



//
//
//
template <typename TypeOne, typename TypeTwo, typename TypeThree>
class ProxyClass
{
public:

    ProxyClass(TypeOne& _one, TypeTwo& _two, TypeThree& _three) :
        one(_one),
        two(_two),
        three(_three)
    {

    }

    void DoThis(uint32_t id)
    {
        switch(id)
        {
            case 11:  one.DoThis();break;
            case 12:  two.DoThis();break;
            case 103: three.DoThis();break;
        }
    }

    void DoThat(uint32_t id)
    {
        switch(id)
        {
            case 11:  one.DoThat();break;
            case 12:  two.DoThat();break;
            case 103: three.DoThat();break;
        }
    }

private:

    TypeOne&    one;
    TypeTwo&    two;
    TypeThree&  three;

};











//
//
//
auto instanceContainer = std::make_tuple( one, two, three );



constexpr int blaa(size_t i, size_t n)
{
    return ((i<n) ? std::get<1>(instanceContainer).DoThat(),blaa(i+1,n) : 0 );
}





template< uint32_t I = 0, typename... Tp>
typename std::enable_if<I == sizeof...(Tp), void>::type blaaa(std::tuple<Tp...>& t)
{ 
}

template< uint32_t I = 0, typename... Tp>
typename std::enable_if<I < sizeof...(Tp), void>::type blaaa(std::tuple<Tp...>& t)
{
    std::get<I>(t).DoThat();    
    if( I<sizeof...(Tp) ) blaaa<I + 1, Tp...>(t);
}



//
//
//
int main()
{
    //CallEach( container );
    //CallIndexed<2>( container );
    //CallEnumerated<2>( container2, 12 );
    //CallEnumeratedTwo<NUMBER_OF_ELEMENTS(idArray), idArray, fnArray>( 103 );
    //Switcher(103);
    //DoSomething( Switcher );

    //ProxyClass<One,Two,Three>  proxy(one, two, three);
    //proxy.DoThis(11);
    //proxy.DoThat(12);

    
    blaaa(instanceContainer);
}



