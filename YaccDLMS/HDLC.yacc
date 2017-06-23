%{
#include <stdio.h>
#include <string.h>

int yylex(void);
int yyparse(void);
 
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 
  
int main()
{
    yyparse();
    return 0;
} 

%}

%token PD BYTE
%start pdu

%%

pdu: PD bytelist PD     {printf("PDU\n");};

bytelist:   BYTE BYTE
        | bytelist BYTE;


%%


