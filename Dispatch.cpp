


#include <stdint.h>
#include <stdio.h>





class One
{
public:
    void operator()()
    {
        printf("One.\n");
    }
};



class Two
{
public:
    void operator()()
    {
        printf("Two.\n");
    }
};



class Three
{
public:
    void operator()()
    {
        printf("Three.\n");
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

    void ShowAll()
    {
        Show<T...>();
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
    template <typename First> void Show() 
    {
        storage[0]   = (void*)1;
    }

    template <typename First, typename Second, typename... Rest> void Show() 
    {
        storage[sizeof...(Rest)]   = (void*)1;
        Show<Second,Rest...>();
    }


    void*       storage[sizeof...(T)];

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
    uint8_t     a0  = 1;
    uint16_t    a1  = 2;
    uint32_t    a2  = 3;

    //IndexedDispatch(0,  one,two,three);

    Container<uint8_t, uint16_t, uint32_t>  container( a0,a1,a2 );
    container.ShowAll();


    volatile ObjectType  order[]     = {eThree, eTwo, eOne};

    for(int i=0; i<NUMBER_OF_ELEMENTS(order); i++)
    {
        EnumeratedDispatch(order[i],    eOne,   one, 
                                        eTwo,   two, 
                                        eThree, three);

    }
    return 0;
}


