%{

#define YYSTYPE char*

#include <stdio.h>
#include "y.tab.h"



%}

%%
7[eE]                   return PD;
[0-9A-F][13579BDF]      yylval=strdup(yytext); return ODDBYTE;
[0-9A-F][02468ACE]      yylval=strdup(yytext); return EVENBYTE;
%%



