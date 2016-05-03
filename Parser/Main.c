

#include <stdio.h>
#include "y.tab.h"


int yylex();


//
//
//
void yyerror(const char *s)
{
    fflush(stdout);
    fprintf(stderr, "*** %s\n", s);
}



//
//
//
int main(int argc, char** argv)
{
    yylex();
}


