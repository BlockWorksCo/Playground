


#include <stdint.h>
#include <stdio.h>
#include <tuple>
#include <utility>





#if 0

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



Container<int,  [](int p0)  {printf("[One:%d]\n",p0);},
                [](int p0)  {printf("[Two:%d]\n",p0);},
                [](int p0)  {printf("[Three:%d]\n",p0);} >  containerA();


#endif

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
  print(std::tuple<Tp...>& t)
  { }

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
  print(std::tuple<Tp...>& t)
  {
    std::get<I>(t)();
    print<I + 1, Tp...>(t);
  }



//
//
//
int main()
{

    auto protocol = std::make_tuple(
            []()  {printf("[One:]\n");},
            []()  {printf("[Two:]\n");},
            []()  {printf("[Three:]\n");}
        );
 

    print( protocol );
}



