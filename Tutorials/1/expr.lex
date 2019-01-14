%{
#include <stdio.h>
#include <stdlib.h>
int yyerror(char *s);
%}

%option noyywrap

%% // begin tokens

[ \n\t]                   // just ignore it


"+"                       { printf("PLUS "); }
"-"                       { printf("MINUS "); }

"//"[^\n]*                { printf("COMMENT "); }

.                         { yyerror("Illegal character!"); yyterminate(); }

%% // end tokens

int yyerror(char *s)
{
  fprintf(stderr,"%d: %s %s\n", yylineno, s, yytext);
  return 0;
}
