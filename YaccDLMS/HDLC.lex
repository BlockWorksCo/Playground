%{

#define YYSTYPE char*

#include <stdio.h>
#include "y.tab.h"



%}

%%
7[eE]           return PD;
[0-9a-fA-F]{2}  yylval=strdup(yytext); return BYTE;
%%



