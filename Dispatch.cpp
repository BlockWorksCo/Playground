


#include <stdint.h>
#include <stdio.h>





class One
{
public:
    void operator()()
    {
        printf("One.\n");
    }

    void DoSomething()
    {
        printf("<One>.\n");
    }
};



class Two
{
public:
    void operator()()
    {
        printf("Two.\n");
    }

    void DoSomething()
    {
        printf("<Two>.\n");
    }
};



class Three
{
public:
    void operator()()
    {
        printf("Three.\n");
    }

    void DoSomething()
    {
        printf("<Three>.\n");
    }
};








template <typename First> void IndexedDispatch(uint8_t index, First& first) 
{
    if(index == 0)
    {
        first();
    }
}

template <typename First, typename... Rest> void IndexedDispatch(uint8_t index, First& first, Rest&... rest) 
{
    if(sizeof...(Rest) == index)
    {
        first();
    }
    IndexedDispatch(index, rest...);
}




#if 0

template <typename First> void EnumeratedDispatch(const int enumeration, const int firstEnumeration, First& first) 
{
    if(enumeration == firstEnumeration)
    {
        first();
    }
}

template <typename First, typename... Rest> void EnumeratedDispatch(const int enumeration, const int firstEnumeration, First& first, Rest&... rest) 
{
    if(enumeration == firstEnumeration)
    {
        first();
    }
    EnumeratedDispatch(enumeration, rest...);
}


#endif







template <typename First, typename EnumerationType> void EnumeratedDispatch(const EnumerationType enumeration, const EnumerationType firstEnumeration, First first) 
{
    if(enumeration == firstEnumeration)
    {
        first();
    }
}

template <typename First, typename EnumerationType, typename... Rest> void EnumeratedDispatch(const EnumerationType enumeration, const EnumerationType firstEnumeration, First& first, Rest... rest) 
{
    if(enumeration == firstEnumeration)
    {
        first();
    }
    EnumeratedDispatch(enumeration, rest...);
}









template <typename... T>
class Container
{

public:

    Container(T... values)
    {
        Store(values...);
    }

    void Call(uint8_t index)
    {
        Call<T...>(index);
    }

private:

    //
    //
    //
    template <typename First> void Store(First& first) 
    {
        storage[0]   = &first;
    }

    template <typename First, typename... Rest> void Store(First& first, Rest&... rest) 
    {
        storage[sizeof...(Rest)]   = &first;
        Store(rest...);
    }

    //
    //
    //
    template <typename First> void Call(uint8_t index) 
    {
        if( index == (sizeof...(T)-1) ) 
        {
            First*  pFirst  = (First*)storage[0];
            First   value   = *pFirst;
            value();
        }
    }

    template <typename First, typename Second, typename... Rest> void Call(uint8_t index) 
    {
        if( index == (sizeof...(T) - (sizeof...(Rest)+1)) - 1)
        {
            First*  pFirst  = (First*)storage[sizeof...(Rest)+1];
            First   value   = *pFirst;
            value();
        }
        else
        {            
            Call<Second,Rest...>(index);
        }
    }

private:

    const void*       storage[sizeof...(T)];

};







//
//
//
template <typename TargetType, void (TargetType::*targetMethod)() >
struct Delegate
{                                                              
    Delegate(TargetType& _instance) : instance(_instance) {}   
    void operator()() { (instance.*targetMethod)(); }                
    TargetType&  instance;                                     
};






#define NUMBER_OF_ELEMENTS(a)   sizeof(a)/sizeof(a[0])



typedef enum 
{
    eOne,
    eTwo,
    eThree,
} ObjectType;



int main()
{
    One         one;
    Two         two;
    Three       three;
    Delegate<One,   &One::DoSomething>      delegateOne(one);
    Delegate<Two,   &Two::DoSomething>      delegateTwo(two);
    Delegate<Three, &Three::DoSomething>    delegateThree(three);
    Container<Delegate<One, &One::DoSomething>, Delegate<Two, &Two::DoSomething>, Delegate<Three, &Three::DoSomething>>  delegateContainer( delegateOne, delegateTwo, delegateThree );
    
    delegateContainer.Call(0);
    delegateContainer.Call(1);
    delegateContainer.Call(2);

    //IndexedDispatch(0,  one,two,three);
#if 0

    volatile ObjectType  order[]     = {eThree, eTwo, eOne};

    for(int i=0; i<NUMBER_OF_ELEMENTS(order); i++)
    {
        EnumeratedDispatch(order[i],    eOne,   one, 
                                        eTwo,   two, 
                                        eThree, three);

    }
#endif    
    return 0;
}


