


#include <stdint.h>
#include <stdio.h>
#include <tuple>




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

    int DoAnotherThing(int p0)
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

    int DoAnotherThing(int p0)
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

    int DoAnotherThing(int p0)
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
            return value(0);
        }
    }

    template <typename First, typename Second, typename... Rest> ReturnType Call(uint8_t index) 
    {
        if( index == (sizeof...(T) - (sizeof...(Rest)+1)) - 1)
        {
            First*  pFirst  = (First*)storage[sizeof...(Rest)+1];
            First   value   = *pFirst;
            return value(0);
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
template <typename ReturnType, typename TargetType, ReturnType (TargetType::*targetMethod)(int) >
struct Delegate
{                                                              
    Delegate(TargetType& _instance) : instance(_instance) {}   
    template <typename... T> ReturnType operator()(T... params) { return (instance.*targetMethod)(params...); }                
    TargetType&  instance;                                     
};



int blaa(int p0, char* p1, int p2)
{
    printf("%d %s %d\n",p0, p1, p2);
}


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

    Delegate<int, One,     &One::DoAnotherThing>       oneDoAnotherThing(one);
    Delegate<int, Two,     &Two::DoAnotherThing>       twoDoAnotherThing(two);
    Delegate<int, Three,   &Three::DoAnotherThing>     threeDoAnotherThing(three);
    Container<int, Delegate<int, One, &One::DoAnotherThing>, Delegate<int, Two, &Two::DoAnotherThing>, Delegate<int, Three, &Three::DoAnotherThing> >  anotherDelegateContainer( oneDoAnotherThing, twoDoAnotherThing, threeDoAnotherThing );

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


