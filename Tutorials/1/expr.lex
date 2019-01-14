%{
#include <stdio.h>
#include <stdlib.h>
#include "expr.y.h"
int yyerror(char *s);
%}

%option noyywrap

%% // begin tokens

[ \n\t]                   // just ignore it

[rR][0-7]                 {  /*printf("REG (%d) ", atoi(yytext+1) );*/ 
                             yylval.reg = atoi(yytext+1); return REG; }

[0-9]+                    {  /* printf("IMMEDIATE (%d) ", atoi(yytext)); */ 
                             yylval.imm = atoi(yytext); return IMMEDIATE; }

"="                       { /*printf("ASSIGN ");*/ return ASSIGN; }
";"                       { /*printf("SEMI ");*/ return SEMI; }

"("                       { /*printf("LPAREN ");*/ return LPAREN; }
")"                       { /*printf("RPAREN ");*/ return RPAREN; }

"["                       { /*printf("LBRACKET ");*/ return LBRACKET; }
"]"                       { /*printf("RBRACKET ");*/ return RBRACKET; }

"+"                       { /*printf("PLUS ");*/ return PLUS; }
"-"                       { /* printf("MINUS ");*/ return MINUS; }

"//"[^\n]*                { /*printf("COMMENT ");*/ }


.                         { yyerror("Illegal character!"); yyterminate(); }

%% // end tokens


int yyerror(char *s)
{
  fprintf(stderr,"%d: %s %s\n", yylineno, s, yytext);
  return 0;
}
