%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


int yyerror(const char *s)
{
  fprintf(stderr,"%d: %s %s\n", yylineno, s, yytext);
  return 0;
}
