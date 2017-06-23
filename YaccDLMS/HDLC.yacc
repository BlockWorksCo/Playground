%{
#define YYSTYPE char *

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

int yylex(void);
int yyparse(void);


char* ConcatenateStrings(char* s1, char* s2)
{
    uint32_t    l1  = strlen(s1);
    uint32_t    l2  = strlen(s2);
    char*       newString   = (char*)malloc(l1+l2);
    memset(newString, 0x00, l1+l2);
    strcpy(newString,s1);
    strcat(newString,s2);
    return newString;
}

 
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
%start pdulist

%%

pdulist:    pdu         
        |   pdulist pdu;

pdu: PD bytelist PD     {printf("PDU(%s)\n", $2);};

bytelist: BYTE              
        | bytelist BYTE     {$$=ConcatenateStrings($1,$2);};


%%


