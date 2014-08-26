


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

    int DoAnotherThing()
    {
        return 11;
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

    int DoAnotherThing()
    {
        return 22;
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

    int DoAnotherThing()
    {
        return 33;
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









template <typename ReturnType, typename... T>
class Container
{

public:

    Container(T... values)
    {
        Store(values...);
    }

    template <typename... ParamTypes> ReturnType Call(uint8_t index, ParamTypes... params)
    {
        return Call<T...>(index);
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
    template <typename First> ReturnType Call(uint8_t index) 
    {
        if( index == (sizeof...(T)-1) ) 
        {
            First*  pFirst  = (First*)storage[0];
            First   value   = *pFirst;
            return value();
        }
    }

    template <typename First, typename Second, typename... Rest> ReturnType Call(uint8_t index) 
    {
        if( index == (sizeof...(T) - (sizeof...(Rest)+1)) - 1)
        {
            First*  pFirst  = (First*)storage[sizeof...(Rest)+1];
            First   value   = *pFirst;
            return value();
        }
        else
        {            
            return Call<Second,Rest...>(index);
        }
    }

private:

    const void*       storage[sizeof...(T)];

};






//
//
//
template <typename ReturnType, typename TargetType, ReturnType (TargetType::*targetMethod)() >
struct Delegate
{                                                              
    Delegate(TargetType& _instance) : instance(_instance) {}   
    template <typename... T> ReturnType operator()(T... params) { return (instance.*targetMethod)(params...); }                
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
    Delegate<void, One,   &One::DoSomething>      delegateOne(one);
    Delegate<void, Two,   &Two::DoSomething>      delegateTwo(two);
    Delegate<void, Three, &Three::DoSomething>    delegateThree(three);    
    Container<void, Delegate<void, One, &One::DoSomething>, Delegate<void, Two, &Two::DoSomething>, Delegate<void, Three, &Three::DoSomething>>  delegateContainer( delegateOne, delegateTwo, delegateThree );

    Delegate<int, One,     &One::DoAnotherThing>       oneDoAnotherThing(one);
    Delegate<int, Two,     &Two::DoAnotherThing>       twoDoAnotherThing(two);
    Delegate<int, Three,   &Three::DoAnotherThing>     threeDoAnotherThing(three);
    Container<int, Delegate<int, One,     &One::DoAnotherThing>, Delegate<int, Two,     &Two::DoAnotherThing>, Delegate<int, Three,   &Three::DoAnotherThing> >  anotherDelegateContainer( oneDoAnotherThing, twoDoAnotherThing, threeDoAnotherThing );

    delegateContainer.Call(0);
    delegateContainer.Call(1);
    delegateContainer.Call(2);

    printf("<%d>\n", anotherDelegateContainer.Call(0) );
    printf("<%d>\n", anotherDelegateContainer.Call(1) );
    printf("<%d>\n", anotherDelegateContainer.Call(2) );

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


