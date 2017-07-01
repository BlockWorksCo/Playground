%{
#define YYSTYPE char *

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

int yylex(void);
int yyparse(void);

int yydebug=1;
 

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

 
char* Concatenate4Strings(char* s1, char* s2, char* s3, char* s4)
{
    uint32_t    l1  = strlen(s1);
    uint32_t    l2  = strlen(s2);
    uint32_t    l3  = strlen(s3);
    uint32_t    l4  = strlen(s4);
    char*       newString   = (char*)malloc(l1+l2+l3+l4);
    memset(newString, 0x00, l1+l2);
    strcpy(newString,s1);
    strcat(newString,s2);
    strcat(newString,s3);
    strcat(newString,s4);
    return newString;
}



char* PayloadFromPayloadAndChecksum(char* payloadAndChecksum)
{
    uint32_t    l  = strlen(payloadAndChecksum);
    char*       newString   = (char*)malloc(l-4);
    memset(newString, 0x00, l-4);
    memcpy(newString, payloadAndChecksum, l-4);
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

%token PD EVENBYTE ODDBYTE SNRMType UAType CRC16
%start framelist

%glr-parser
%%

BYTE:           ODDBYTE
        |       EVENBYTE;

framelist:      frame         
         |      framelist frame;

frame:          SNRMframe
        |       UAframe;

SNRMframe:      PD frameType frameLength address address SNRMType bytelist PD       {printf("SNRMframe(ft=%s,fl=%s,add1=%s,add2=%s,frame=%s,payload=%s)\n", $2,$3,$4,$5,$6,$7); $$=PayloadFromPayloadAndChecksum($6);};

UAframe:        PD frameType frameLength address address UAType CRC16 bytelist PD         {printf("UAframe(ft=%s,fl=%s,add1=%s,add2=%s,frame=%s,CRC=%s,payload=%s)\n", $2,$3,$4,$5,$6,$7,$8); $$=PayloadFromPayloadAndChecksum($8);};

frameLength:    BYTE;

frameType:      BYTE;

address:        ODDBYTE
        |       EVENBYTE ODDBYTE                        {$$=ConcatenateStrings($1,$2);}; 
        |       EVENBYTE EVENBYTE EVENBYTE ODDBYTE      {$$=Concatenate4Strings($1,$2,$3,$4);}; ;

bytelist:       BYTE              
        |       bytelist BYTE                           {$$=ConcatenateStrings($1,$2);};


%%


