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

%type <reg> REG expr
%type <imm> IMMEDIATE

%start program


%%

program: REG ASSIGN expr SEMI 
{
  printf("# Output program : REG (%d) ASSIGN expr SEMI\n", $1);
  printf("ADD R%d, R%d, 0\n", $1, $3);
} 
| program REG ASSIGN expr SEMI
{
  printf("# Output program : REG (%d) ASSIGN expr SEMI\n", $2);
  printf("ADD R%d, R%d, 0\n", $2, $4);

} 

| program SEMI
{
  return 0;
}
;
expr: 
 IMMEDIATE                 
 {
   int reg = regCnt++;
   printf("# Output for expr : IMMEDIATE (%d) \n", $1);
   printf("AND R%d, R%d, 0\n", reg, reg);
   printf("ADD R%d, R%d, %d\n", reg, reg, $1);
   $$ = reg;
 }
| REG
 {
   printf("# Output expr : REG (%d) \n",$1);
   $$ = $1;
 }
| expr PLUS expr  
 {
  printf("# Output expr : expr PLUS expr \n");
  int reg = regCnt++;
  printf("ADD R%d, R%d, R%d\n", reg, $1, $3);
  $$ = reg;
 }

| expr MINUS expr 
 {
  printf("# Output for expr : expr MINUS expr \n");

  int reg = regCnt++;
  printf("SUB R%d, R%d, R%d\n", reg, $1, $3);
  $$ = reg;  
}


| LPAREN expr RPAREN 
 {
  printf("# Output for expr : LPAREN expr RPAREN \n");
  $$ = $2;
 }

| MINUS expr 
 {
  printf("expr : MINUS expr \n");
  
  int reg = regCnt++;
  printf("NOT R%d, R%d\n", reg, $2);
  printf("ADD R%d, R%d, 1\n", reg, reg);
  $$ = reg;  
 }

| LBRACKET expr RBRACKET 
 {
  printf("#Output for expr : LBRACKET expr RBRACKET \n");

  int reg = regCnt++;
  printf("LDR R%d, R%d, 0\n", reg, $2);
  $$ = reg;  
 }

;

%%
