%{

#define YYSTYPE char*

#include <stdio.h>
#include "y.tab.h"



%}

%%
73                      yylval=strdup(yytext); return UAType;
93                      yylval=strdup(yytext); return SNRMType;
83                      yylval=strdup(yytext); return SNRMType;
7[eE]                   return PD;
[0-9A-F][13579BDF]      yylval=strdup(yytext); return ODDBYTE;
[0-9A-F][02468ACE]      yylval=strdup(yytext); return EVENBYTE;
[0-9A-F]{4}             yylval=strdup(yytext); return CRC16;
%%



