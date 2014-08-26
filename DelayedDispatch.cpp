


#include <stdio.h>
#include <tuple>



//
//
//
template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };


//
//
//
template < typename ReturnType, typename TargetType, typename ...Args>
struct Delegate
{

    Delegate(TargetType& _targetInstance, ReturnType (TargetType::*_func)(Args...), std::tuple<Args...> _params) :
        targetInstance(_targetInstance),
        func(_func),
        params(_params)
    {

    }

    ReturnType operator()()
    {
        return callFunc(typename gens<sizeof...(Args)>::type());
    }


private:

    template<int ...S>
    ReturnType callFunc(seq<S...>)
    {
        return (targetInstance.*func)(std::get<S>(params) ...);
    }


    std::tuple<Args...>     params;
    ReturnType              (TargetType::*func)(Args...);
    TargetType&             targetInstance;
};








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






//
//
//
int main(void)
{
    One           one;
    Two           two;
    Three         three;    
    Delegate<int, One, int>       delegateOne(one, &One::DoAnotherThing, std::make_tuple(1) );
    Delegate<int, Two, int>       delegateTwo(two, &Two::DoAnotherThing, std::make_tuple(1) );
    Delegate<int, Three, int>     delegateThree(three, &Three::DoAnotherThing, std::make_tuple(1) );
    Container<int, Delegate<int, One, int>, Delegate<int, Two, int>, Delegate<int, Three, int> >  anotherDelegateContainer( delegateOne, delegateTwo, delegateThree );

    //
    //
    //
    //printf("<%d>\n", delegateOne() );
    //printf("<%d>\n", delegateTwo() );
    //printf("<%d>\n", delegateThree() );

    //
    //
    //
    printf("<%d>\n", anotherDelegateContainer.Call(0) );
    printf("<%d>\n", anotherDelegateContainer.Call(1) );
    printf("<%d>\n", anotherDelegateContainer.Call(2) );

}


