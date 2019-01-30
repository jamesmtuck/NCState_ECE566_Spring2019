%{
#include <stdio.h>
#include <stdlib.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;

#include "expr.y.hpp"
int yyerror(char *s);

int yylex();
int yyerror(const char *);


%}

%option noyywrap noinput nounput

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


int yyerror(const char *s)
{
  fprintf(stderr,"%d: %s %s\n", yylineno, s, yytext);
  return 0;
}
