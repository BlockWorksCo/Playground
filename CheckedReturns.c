

#include <stdio.h>



void FuncC()
{
    printf("C\n");
}



void FuncB()
{
    printf("B\n");
    FuncC();
}





void FuncA()
{
    printf("A\n");
    FuncB();
}



int main()
{
    printf("CheckedReturns\n");
    FuncA();
}



