


#include <stdio.h>
#include <tuple>



//
// Originally from:
// http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
//
template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };


//
//
//
template < typename ReturnType, typename TargetType, typename ...Args>
class Delegate
{
public:
    Delegate(TargetType& _targetInstance, ReturnType (TargetType::*_func)(Args...) ) :
        targetInstance(_targetInstance),
        func(_func)
    {
    }

    ReturnType operator()(Args... params)
    {
        return Call( std::make_tuple(params... ) );
    }

    ReturnType Call(std::tuple<Args...> params)
    {
        return callFunc(params, typename gens<sizeof...(Args)>::type());        
    }


private:

    template<int ...S>
    ReturnType callFunc(std::tuple<Args...> params, seq<S...>)
    {
        return (targetInstance.*func)(std::get<S>(params) ...);
    }


    ReturnType              (TargetType::*func)(Args...);
    TargetType&             targetInstance;
};








template <typename ReturnType, typename ParamTypes ,typename... T>
class Container
{

public:

    Container(T... values)
    {
        Store(values...);
    }

    ReturnType Call(uint8_t index, ParamTypes _params)
    {
        return Call<T...>(index, _params);
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
    template <typename First> ReturnType Call(uint8_t index, ParamTypes params) 
    {
        if( index == (sizeof...(T)-1) ) 
        {
            First*  pFirst  = (First*)storage[0];
            First   value   = *pFirst;
            return value.Call(params);
        }
    }

    template <typename First, typename Second, typename... Rest> ReturnType Call(uint8_t index, ParamTypes params) 
    {
        if( index == (sizeof...(T) - (sizeof...(Rest)+1)) - 1)
        {
            First*  pFirst  = (First*)storage[sizeof...(Rest)+1];
            First   value   = *pFirst;
            return value.Call(params);
        }
        else
        {            
            return Call<Second,Rest...>(index, params);
        }
    }

private:

    const void*       storage[sizeof...(T)];

};

















volatile uint8_t    v0  = 0;
volatile uint8_t    v1  = 0;
volatile uint8_t    v2  = 0;


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
        return p0*11;
    }

    void Simple(uint8_t p0)
    {
        v0  = p0;
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
        return p0*11;
    }

    void Simple(uint8_t p0)
    {
        v1  = p0;
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
        return p0*11;
    }

    void Simple(uint8_t p0)
    {
        v2  = p0;
    }
};





One           one;
Two           two;
Three         three;    

//
//
//
int main(void)
{
    typedef std::tuple<int>    ParamTypes;
    Delegate<int, One, int>       delegateOne(one, &One::DoAnotherThing );
    Delegate<int, Two, int>       delegateTwo(two, &Two::DoAnotherThing );
    Delegate<int, Three, int>     delegateThree(three, &Three::DoAnotherThing );
    Container<int,ParamTypes, Delegate<int, One, int>, Delegate<int, Two, int>, Delegate<int, Three, int> >  anotherDelegateContainer( delegateOne, delegateTwo, delegateThree );


    Delegate<void, One, uint8_t>       simpleOne(one, &One::Simple );
    Delegate<void, Two, uint8_t>       simpleTwo(two, &Two::Simple );
    Delegate<void, Three, uint8_t>     simpleThree(three, &Three::Simple );
    Container<void,ParamTypes,  Delegate<void, One, uint8_t>, 
                                Delegate<void, Two, uint8_t>, 
                                Delegate<void, Three, uint8_t> >  simpleContainer(  simpleOne, 
                                                                                    simpleTwo, 
                                                                                    simpleThree );

    simpleContainer.Call(0, std::make_tuple(99) );

    //
    //
    //
    //printf("<%d>\n", delegateOne(1) );
    //printf("<%d>\n", delegateTwo(2) );
    //printf("<%d>\n", delegateThree(3) );

    //
    //
    //
    //printf("<%d>\n", anotherDelegateContainer.Call(0, std::make_tuple(1) ) );
    //printf("<%d>\n", anotherDelegateContainer.Call(1, std::make_tuple(2) ) );
    //printf("<%d>\n", anotherDelegateContainer.Call(2, std::make_tuple(3) ) );

}


