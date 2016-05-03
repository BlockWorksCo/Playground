
%{
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
int yylex();
void yyerror(const char *s);
int yylineno;


%}

%token <text> IDENTIFIER 
%token <text> I_CONSTANT 
%token <text> F_CONSTANT 
%token <text> STRING_LITERAL 
%token <text> SIZEOF
%token <text> INC_OP 
%token <text> DEC_OP 
%token <text> LEFT_OP 
%token <text> RIGHT_OP 
%token <text> LE_OP 
%token <text> GE_OP 
%token <text> EQ_OP 
%token <text> NE_OP
%token <text> AND_OP 
%token <text> OR_OP 
%token <text> MUL_ASSIGN 
%token <text> DIV_ASSIGN 
%token <text> MOD_ASSIGN 
%token <text> ADD_ASSIGN
%token <text> SUB_ASSIGN 
%token <text> LEFT_ASSIGN 
%token <text> RIGHT_ASSIGN 
%token <text> AND_ASSIGN
%token <text> XOR_ASSIGN 
%token <text> OR_ASSIGN
%token <text> TYPEDEF_NAME 
%token <text> ENUMERATION_CONSTANT
%token <text> TYPEDEF 
%token <text> EXTERN 
%token <text> STATIC
%token <text> CONST 
%token <text> RESTRICT 
%token <text> VOLATILE
%token <text> BOOL 
%token <text> VOID
%token <text> FLOAT32 
%token <text> FLOAT64
%token <text> INT8 
%token <text> INT16 
%token <text> INT32 
%token <text> INT64
%token <text> UINT8 
%token <text> UINT16 
%token <text> UINT32 
%token <text> UINT64
%token <text> STRUCT 
%token <text> ENUM 
%token <text> ELLIPSIS
%token <text> CASE 
%token <text> DEFAULT 
%token <text> IF 
%token <text> ELSE 
%token <text> SWITCH 
%token <text> FOR 
%token <text> RETURN

%union 
{
        int     number;
        char    text[256];
}



%start translation_unit
%%

primary_expression
    : IDENTIFIER
    | constant
    | string
    | '(' expression ')'
    ;

constant
    : I_CONSTANT        /* includes character_constant */
    | F_CONSTANT
    | ENUMERATION_CONSTANT  /* after it has been defined as such */
    ;

enumeration_constant        /* before it has been defined as such */
    : IDENTIFIER
    ;

string
    : STRING_LITERAL
    ;


postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']'
    | postfix_expression '(' ')'
    | postfix_expression '(' argument_expression_list ')'
    | postfix_expression '.' IDENTIFIER
    | postfix_expression INC_OP
    | postfix_expression DEC_OP
    | '(' type_name ')' '{' initializer_list '}'
    | '(' type_name ')' '{' initializer_list ',' '}'
    ;

argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;

unary_expression
    : postfix_expression
    | INC_OP unary_expression
    | DEC_OP unary_expression
    | unary_operator cast_expression
    | SIZEOF unary_expression
    | SIZEOF '(' type_name ')'
    ;

unary_operator
    : '&'
    | '*'
    | '+'
    | '-'
    | '~'
    | '!'
    ;

cast_expression
    : unary_expression
    | '(' type_name ')' cast_expression
    ;

multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression
    | multiplicative_expression '/' cast_expression
    | multiplicative_expression '%' cast_expression
    ;

additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;

shift_expression
    : additive_expression
    | shift_expression LEFT_OP additive_expression
    | shift_expression RIGHT_OP additive_expression
    ;

relational_expression
    : shift_expression
    | relational_expression '<' shift_expression
    | relational_expression '>' shift_expression
    | relational_expression LE_OP shift_expression
    | relational_expression GE_OP shift_expression
    ;

equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression AND_OP inclusive_or_expression
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression OR_OP logical_and_expression
    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression
    ;

assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression
    ;

assignment_operator
    : '='                                                   {printf("<ASSIGN op>\n");}
    | MUL_ASSIGN  
    | DIV_ASSIGN
    | MOD_ASSIGN
    | ADD_ASSIGN
    | SUB_ASSIGN
    | LEFT_ASSIGN
    | RIGHT_ASSIGN
    | AND_ASSIGN
    | XOR_ASSIGN
    | OR_ASSIGN
    ;

expression
    : assignment_expression                                 {printf("<assign>\n");}
    | expression ',' assignment_expression                  {printf("<expression>\n");}
    ;

constant_expression
    : conditional_expression    /* with constraints */
    ;

declaration
    : declaration_specifiers ';'                            {printf("<declaration1>\n");}
    | declaration_specifiers init_declarator_list ';'       {printf("<declaration2>\n");}
    ;

declaration_specifiers
    : storage_class_specifier declaration_specifiers        {printf("<5>\n");}
    | storage_class_specifier                               {printf("<6>\n");}
    | type_specifier declaration_specifiers                 {printf("<7>\n");}
    | type_specifier                                        {printf("<%s>\n",yylval.text);}
    | type_qualifier declaration_specifiers                 {printf("<9>\n");}
    | type_qualifier                                        {printf("<10>\n");}
    ;

init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;

init_declarator
    : declarator '=' initializer
    | declarator
    ;

storage_class_specifier
    : TYPEDEF   /* identifiers must be flagged as TYPEDEF_NAME */
    | EXTERN
    ;

type_specifier
    : BOOL                                                  /*{printf("<%s>\n",$1);}*/
    | INT8                                                  /*{printf("<%s>\n",$1);}*/
    | INT16                                                 /*{printf("<%s>\n",$1);}*/
    | VOID                                                  /*{printf("<%s>\n",$1);}*/
    | INT32                                                 {printf("<%s>\n",$1);}
    | INT64                                                 /*{printf("<%s>\n",$1);}*/
    | UINT8                                                 /*{printf("<%s>\n",$1);}*/
    | UINT16                                                /*{printf("<%s>\n",$1);}*/
    | UINT32                                                /*{printf("<%s>\n",$1);}*/
    | UINT64                                                /*{printf("<%s>\n",$1);}*/
    | struct_specifier                                      /*{printf("<%s>\n",$1);}*/
    | enum_specifier                                        /*{printf("<%s>\n",$1);}*/
    | TYPEDEF_NAME                                          /*{printf("<%s>\n",$1);}*/
    ;

struct_specifier
    : struct '{' struct_declaration_list '}'
    | struct IDENTIFIER '{' struct_declaration_list '}'
    | struct IDENTIFIER
    ;

struct
    : STRUCT
    ;

struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;

struct_declaration
    : specifier_qualifier_list ';'  /* for anonymous struct/union */
    | specifier_qualifier_list struct_declarator_list ';'
    ;

specifier_qualifier_list
    : type_specifier specifier_qualifier_list
    | type_specifier
    | type_qualifier specifier_qualifier_list
    | type_qualifier
    ;

struct_declarator_list
    : struct_declarator
    | struct_declarator_list ',' struct_declarator
    ;

struct_declarator
    : ':' constant_expression
    | declarator ':' constant_expression
    | declarator
    ;

enum_specifier
    : ENUM '{' enumerator_list '}'
    | ENUM '{' enumerator_list ',' '}'
    | ENUM IDENTIFIER '{' enumerator_list '}'
    | ENUM IDENTIFIER '{' enumerator_list ',' '}'
    | ENUM IDENTIFIER
    ;

enumerator_list
    : enumerator
    | enumerator_list ',' enumerator
    ;

enumerator  /* identifiers must be flagged as ENUMERATION_CONSTANT */
    : enumeration_constant '=' constant_expression
    | enumeration_constant
    ;

type_qualifier
    : CONST
    | RESTRICT
    | VOLATILE
    ;

declarator
    : direct_declarator
    ;

direct_declarator
    : IDENTIFIER
    | '(' declarator ')'
    | direct_declarator '[' ']'
    | direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
    | direct_declarator '[' STATIC assignment_expression ']'
    | direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
    | direct_declarator '[' type_qualifier_list assignment_expression ']'
    | direct_declarator '[' type_qualifier_list ']'
    | direct_declarator '[' assignment_expression ']'
    | direct_declarator '(' parameter_type_list ')'
    | direct_declarator '(' ')'
    | direct_declarator '(' identifier_list ')'
    ;

type_qualifier_list
    : type_qualifier
    | type_qualifier_list type_qualifier
    ;


parameter_type_list
    : parameter_list ',' ELLIPSIS
    | parameter_list
    ;

parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;

parameter_declaration
    : declaration_specifiers declarator
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;

identifier_list
    : IDENTIFIER
    | identifier_list ',' IDENTIFIER
    ;

type_name
    : specifier_qualifier_list abstract_declarator
    | specifier_qualifier_list
    ;

abstract_declarator
    : direct_abstract_declarator
    ;

direct_abstract_declarator
    : '(' abstract_declarator ')'
    | '[' ']'
    | '[' STATIC type_qualifier_list assignment_expression ']'
    | '[' STATIC assignment_expression ']'
    | '[' type_qualifier_list STATIC assignment_expression ']'
    | '[' type_qualifier_list assignment_expression ']'
    | '[' type_qualifier_list ']'
    | '[' assignment_expression ']'
    | direct_abstract_declarator '[' ']'
    | direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
    | direct_abstract_declarator '[' STATIC assignment_expression ']'
    | direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
    | direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
    | direct_abstract_declarator '[' type_qualifier_list ']'
    | direct_abstract_declarator '[' assignment_expression ']'
    | '(' ')'
    | '(' parameter_type_list ')'
    | direct_abstract_declarator '(' ')'
    | direct_abstract_declarator '(' parameter_type_list ')'
    ;

initializer
    : '{' initializer_list '}'
    | '{' initializer_list ',' '}'
    | assignment_expression
    ;

initializer_list
    : designation initializer
    | initializer
    | initializer_list ',' designation initializer
    | initializer_list ',' initializer
    ;

designation
    : designator_list '='
    ;

designator_list
    : designator
    | designator_list designator
    ;

designator
    : '[' constant_expression ']'
    | '.' IDENTIFIER
    ;

statement
    : labeled_statement
    | compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;

labeled_statement
    : IDENTIFIER ':' statement
    | CASE constant_expression ':' statement
    | DEFAULT ':' statement
    ;

compound_statement
    : '{' '}'                                                   {printf("<compound_statement1>\n");}
    | '{'  block_item_list '}'                                  {printf("<compound_statement2>\n");}
    ;

block_item_list
    : block_item                                                {printf("<block_item_list1>\n");}
    | block_item_list block_item                                {printf("<block_item_list2>\n");}
    ;

block_item
    : declaration                                               {printf("<block_item1>\n");}
    | statement                                                 {printf("<block_item2>\n");}
    ;

expression_statement
    : ';'
    | expression ';'
    ;

selection_statement
    : IF '(' expression ')' statement ELSE statement
    | IF '(' expression ')' statement
    | SWITCH '(' expression ')' statement
    ;

iteration_statement
    : FOR '(' expression_statement expression_statement ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    | FOR '(' declaration expression_statement ')' statement
    | FOR '(' declaration expression_statement expression ')' statement
    ;

jump_statement
    : RETURN ';'
    | RETURN expression ';'
    ;

translation_unit
    : external_declaration                              {printf("<translation_unit1>\n");}
    | translation_unit external_declaration             {printf("<translation_unit2>\n");}
    ;

external_declaration
    : function_definition                               {printf("<external_declaration1>\n");}
    | declaration                                       {printf("<external_declaration2>\n");}
    ;

function_definition
    : declaration_specifiers declarator declaration_list compound_statement     {printf("<function_definition1>\n");}
    | declaration_specifiers declarator compound_statement                      {printf("<function_definition2>\n");}
    ;

declaration_list
    : declaration                                                               {printf("<declaration_list1>\n");}
    | declaration_list declaration                                              {printf("<declaration_list2>\n");}
    ;

%%


