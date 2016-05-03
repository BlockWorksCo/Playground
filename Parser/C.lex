

%e  1019
%p  2807
%n  371
%k  284
%a  1213
%o  1117

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]

%{
#include <stdio.h>
#include "y.tab.h"

extern void yyerror(const char *);  /* prints grammar violation message */

extern int sym_type(const char *);  /* returns type from symbol table */

#define sym_type(identifier) IDENTIFIER /* with no symbol table, fake it */

static void comment(void);
static int check_type(void);
%}

%%
"/*"                                    { comment(); }
"//".*                                    { /* consume //-comment */ }

"int8_t"               { strcpy(yylval.text, yytext); return(INT8); }
"int16_t"               { strcpy(yylval.text, yytext); return(INT16); }
"int32_t"               { strcpy(yylval.text, yytext); return(INT32); }
"int64_t"               { return(INT64); }

"uint8_t"               { strcpy(yylval.text, yytext); return(UINT8); }
"uint16_t"               { strcpy(yylval.text, yytext); return(UINT16); }
"uint32_t"               { strcpy(yylval.text, yytext); return(UINT32); }
"uint64_t"               { strcpy(yylval.text, yytext); return(UINT64); }

"case"                  { strcpy(yylval.text, yytext); return(CASE); }
"const"                 { strcpy(yylval.text, yytext); return(CONST); }
"default"               { strcpy(yylval.text, yytext); return(DEFAULT); }
"float64"               { strcpy(yylval.text, yytext); return(FLOAT64); }
"else"                  { strcpy(yylval.text, yytext); return(ELSE); }
"enum"                  { strcpy(yylval.text, yytext); return(ENUM); }
"extern"                { strcpy(yylval.text, yytext); return(EXTERN); }
"float32"               { strcpy(yylval.text, yytext); return(FLOAT32); }
"for"                   { strcpy(yylval.text, yytext); return(FOR); }
"if"                    { strcpy(yylval.text, yytext); return(IF); }
"restrict"              { strcpy(yylval.text, yytext); return(RESTRICT); }
"return"                { strcpy(yylval.text, yytext); return(RETURN); }
"sizeof"                { strcpy(yylval.text, yytext); return(SIZEOF); }
"struct"                { strcpy(yylval.text, yytext); return(STRUCT); }
"switch"                { strcpy(yylval.text, yytext); return(SWITCH); }
"typedef"               { strcpy(yylval.text, yytext); return(TYPEDEF); }
"void"                  { strcpy(yylval.text, yytext); return(VOID); }
"volatile"              { strcpy(yylval.text, yytext); return(VOLATILE); }

{L}{A}*                 { strcpy(yylval.text, yytext); return check_type(); }

{HP}{H}+{IS}?               { strcpy(yylval.text, yytext); return I_CONSTANT; }
{NZ}{D}*{IS}?               { strcpy(yylval.text, yytext); return I_CONSTANT; }
"0"{O}*{IS}?                { strcpy(yylval.text, yytext); return I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"     { strcpy(yylval.text, yytext); return I_CONSTANT; }

{D}+{E}{FS}?                { strcpy(yylval.text, yytext); return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?            { strcpy(yylval.text, yytext); return F_CONSTANT; }
{D}+"."{E}?{FS}?            { strcpy(yylval.text, yytext); return F_CONSTANT; }
{HP}{H}+{P}{FS}?            { strcpy(yylval.text, yytext); return F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?         { strcpy(yylval.text, yytext); return F_CONSTANT; }
{HP}{H}+"."{P}{FS}?         { strcpy(yylval.text, yytext); return F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+   { strcpy(yylval.text, yytext); return STRING_LITERAL; }

"..."                   { return ELLIPSIS; }
">>="                   { return RIGHT_ASSIGN; }
"<<="                   { return LEFT_ASSIGN; }
"+="                    { return ADD_ASSIGN; }
"-="                    { return SUB_ASSIGN; }
"*="                    { return MUL_ASSIGN; }
"/="                    { return DIV_ASSIGN; }
"%="                    { return MOD_ASSIGN; }
"&="                    { return AND_ASSIGN; }
"^="                    { return XOR_ASSIGN; }
"|="                    { return OR_ASSIGN; }
">>"                    { return RIGHT_OP; }
"<<"                    { return LEFT_OP; }
"++"                    { return INC_OP; }
"--"                    { return DEC_OP; }
"&&"                    { return AND_OP; }
"||"                    { return OR_OP; }
"<="                    { return LE_OP; }
">="                    { return GE_OP; }
"=="                    { return EQ_OP; }
"!="                    { return NE_OP; }
";"                 { return ';'; }
("{"|"<%")              { return '{'; }
("}"|"%>")              { return '}'; }
","                 { return ','; }
":"                 { return ':'; }
"="                 { return '='; }
"("                 { return '('; }
")"                 { return ')'; }
("["|"<:")              { return '['; }
("]"|":>")              { return ']'; }
"."                 { return '.'; }
"&"                 { return '&'; }
"!"                 { return '!'; }
"~"                 { return '~'; }
"-"                 { return '-'; }
"+"                 { return '+'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"%"                 { return '%'; }
"<"                 { return '<'; }
">"                 { return '>'; }
"^"                 { return '^'; }
"|"                 { return '|'; }
"?"                 { return '?'; }

{WS}+                   { /* whitespace separates tokens */ }
.                   { /* discard bad characters */ }

%%

int yywrap(void)        /* called at end of input */
{
    return 1;           /* terminate now */
}

static void comment(void)
{
    int c;

    while ((c = input()) != 0)
        if (c == '*')
        {
            while ((c = input()) == '*')
                ;

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}

static int check_type(void)
{
    switch (sym_type(yytext))
    {
    case TYPEDEF_NAME:                /* previously defined */
        return TYPEDEF_NAME;
    case ENUMERATION_CONSTANT:        /* previously defined */
        return ENUMERATION_CONSTANT;
    default:                          /* includes undefined */
        return IDENTIFIER;
    }
}