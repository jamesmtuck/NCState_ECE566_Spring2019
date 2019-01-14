%{
#include <stdio.h>
%}

%token REG IMMEDIATE ASSIGN SEMI LPAREN RPAREN LBRACKET RBRACKET MINUS PLUS

%left PLUS MINUS

%start program

%%

program: REG ASSIGN expr SEMI 
;

expr: 
 IMMEDIATE                 
| REG
| expr PLUS expr  
| expr MINUS expr 
| LPAREN expr RPAREN 
| MINUS expr 
| LBRACKET expr RBRACKET 
;

%%
