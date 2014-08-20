


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






template <typename First> void EnumeratedDispatch(int enumeration, int& firstEnumeration, First& first) 
{
    if(enumeration == firstEnumeration)
    {
        first();
    }
}

template <typename First, typename... Rest> void EnumeratedDispatch(int enumeration, int& firstEnumeration, First& first, Rest&... rest) 
{
    if(enumeration == firstEnumeration)
    {
        first();
    }
    EnumeratedDispatch(enumeration, rest...);
}




int main()
{
    One     one;
    Two     two;
    Three   three;

    //IndexedDispatch(0,  one,two,three);

    int     a = 11;
    int     b = 22;
    int     c = 33;
    EnumeratedDispatch(33,  a,one, b,two, c,three);

    return 0;
}


