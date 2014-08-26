


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

    int foo(int x, float y, double z)
    {
        printf("%d %f %f\n",x,y,z);
        return x + y + z;
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
    One                                     one;
    Two                                     two;
    Three                                   three;    
    std::tuple<int, float, double>          params    = std::make_tuple(1, 1.2, 5);
    //Delegate<int, One, int,float, double>   delegate  = {params, &One::foo, one};
    Delegate<int, One, int,float, double>   delegate(one, &One::foo, params);

    //
    //
    //
    delegate();
    //printf("%f\n", delegate() );
}


