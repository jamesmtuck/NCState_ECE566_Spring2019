%{
#include <stdio.h>
#include <stdlib.h>

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

#include "expr.y.h"
void yyerror(const char *s);
int yylex();
%}

%option noyywrap noinput nounput

%% // begin tokens

[ \n\t]                   // just ignore it

"while"                     return WHILE;
"if"                        return IF;
"return"                    return RETURN;

[a-zA-Z]+                 { yylval.id = strdup(yytext);  return ID; }
[0-9]+                    { yylval.imm = atoi(yytext);   return IMMEDIATE; }

"="                       { return ASSIGN; }
";"                       { return SEMI; }
"("                       { return LPAREN; }       
")"                       { return RPAREN; }
"{"                       { return LBRACE; }       
"}"                       { return RBRACE; }

"+"                       { return PLUS;}
"-"                       { return MINUS; }
"*"                       { return MULTIPLY; }
"/"                       { return DIVIDE; }
"!"                       { return NOT; }

"//"[^\n]*                { }

.                         { yyerror("Illegal character!"); yyterminate(); }

%% // end tokens

void yyerror(const char *s)
{
  fprintf(stderr,"%d: %s %s\n", yylineno, s, yytext);
}
