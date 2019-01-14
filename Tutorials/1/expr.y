%{
#include <stdio.h>
  int regCnt = 8;
%}

%token REG IMMEDIATE ASSIGN SEMI LPAREN RPAREN LBRACKET RBRACKET MINUS PLUS

%left PLUS MINUS

%union {
  int reg;
  int imm;
}

%type <reg> REG
%type <imm> IMMEDIATE

%start program


%%

program: REG ASSIGN expr SEMI 
{
  printf("program : REG (%d) ASSIGN expr SEMI\n", $1);
  return 0;
}
;

expr: 
 IMMEDIATE                 
 {
   int reg = regCnt++;
   printf("# Output for expr : IMMEDIATE (%d) \n", $1);
   printf("AND R%d, R%d, 0\n", reg);
   printf("ADD R%d, R%d, %d\n", reg, reg, $1);
   //   $$ = reg;
 }
| REG
 {
   printf("expr : REG (%d) \n",$1);
 }

| expr PLUS expr  
 {
  printf("expr : expr PLUS expr \n");
 }

| expr MINUS expr 
 {
  printf("expr : expr MINUS expr \n");
 }


| LPAREN expr RPAREN 
 {
  printf("expr : LPAREN expr RPAREN \n");
 }

| MINUS expr 
 {
  printf("expr : MINUS expr \n");
 }

| LBRACKET expr RBRACKET 
 {
  printf("expr : LBRACKET expr RBRACKET \n");
 }

;

%%
