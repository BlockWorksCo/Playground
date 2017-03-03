




#include <stdio.h>



template < typename CallableType >
class Callable
{
public:
    Callable( CallableType _callable ) :
        callable(_callable)
    {
    }

    void operator()()
    {
        callable();
    }

    CallableType	callable;
};




typedef struct
{
    void operator()()
    {
		printf("Hello Functor.\n");
    }

} TestCallableType;




void TestFn()
{
	printf("Hello World.\n");
}




template <typename CallableType>
void PassThrough( CallableType callable )
{
	callable();
}





auto Blaa = [](int protocolType) -> int { switch(protocolType) { case 1:return one.DoAnotherThing(protocolType);break;   case 2:return two.DoAnotherThing(protocolType);break;  case 3:return three.DoAnotherThing(protocolType);break; } };





void DoIt(int type, int (*func)(int) )
{
    printf("%d\n", func(type) );
}



int main()
{
	TestFn();

	Callable< void (*)() >	testCallable1(TestFn);
	testCallable1();

	TestCallableType	testCallable2;
	testCallable2();

	Callable< TestCallableType >	testCallable3(testCallable2); 
	testCallable3();
}



