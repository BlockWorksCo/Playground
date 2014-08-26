


#include <stdio.h>
#include <tuple>




template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };



int foo(int x, float y, double z)
{
    printf("%d %f %f\n",x,y,z);
    return x + y + z;
}



template < typename ReturnType, typename ...Args>
struct Delegate
{
    std::tuple<Args...>   params;
    ReturnType            (*func)(Args...);

    ReturnType operator()()
    {
        return callFunc(typename gens<sizeof...(Args)>::type());
    }

    template<int ...S>
    ReturnType callFunc(seq<S...>)
    {
        return func(std::get<S>(params) ...);
    }
};



int main(void)
{
    std::tuple<int, float, double>  t         = std::make_tuple(1, 1.2, 5);
    Delegate<int, int,float, double>     delegate  = {t, foo};

    //
    //
    //
    delegate();
    //printf("%f\n", delegate() );
}


