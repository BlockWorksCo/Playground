


#include <stdio.h>
#include <tuple>




template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };



double foo(int x, float y, double z)
{
  printf("%d %f %f\n",x,y,z);
  return x + y + z;
}



template <typename ...Args>
struct save_it_for_later
{
  std::tuple<Args...>   params;
  double                (*func)(Args...);

  double delayed_dispatch()
  {
    return callFunc(typename gens<sizeof...(Args)>::type());
  }

  template<int ...S>
  double callFunc(seq<S...>)
  {
    return func(std::get<S>(params) ...);
  }
};



int main(void)
{
  gens<10>        g;
  gens<10>::type  s;

  std::tuple<int, float, double> t = std::make_tuple(1, 1.2, 5);
  save_it_for_later<int,float, double> saved = {t, foo};
  saved.delayed_dispatch();
}


