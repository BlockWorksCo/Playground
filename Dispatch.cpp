


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

        for(int i=0; i<sizeof...(T); i++)
        {
            printf("%d) %p\n",i,storage[i]);
        }
    }

    void ShowAll()
    {
        for(int i=0; i<sizeof...(T); i++)
        {
            printf("%d) %p\n",i,storage[i]);
        }

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
        First*  pFirst  = (First*)storage[0];
        First   value   = *pFirst;
        value();
    }

    template <typename First, typename Second, typename... Rest> void Show() 
    {
        First*  pFirst  = (First*)storage[sizeof...(Rest)+1];
        First   value   = *pFirst;
        value();
        Show<Second,Rest...>();
    }


private:

    const void*       storage[sizeof...(T)];

};







#define NUMBER_OF_ELEMENTS(a)   sizeof(a)/sizeof(a[0])



typedef enum 
{
    eOne,
    eTwo,
    eThree,
} ObjectType;
    static uint8_t     a0  = 11;
    static uint16_t    a1  = 22;
    static int         a2  = 33;
    static int         a3  = 44;
    static int         a4  = 55;

int main()
{
#if 1
    One         one;
    Two         two;
    Three       three;
#endif    

    //IndexedDispatch(0,  one,two,three);

    //Container<uint8_t, uint16_t, int,int,int>  container( a0,a1,a2,a3,a4 );
    Container<One, Two, Three>  container( one, two, three );
    printf("\n");
    container.ShowAll();
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


