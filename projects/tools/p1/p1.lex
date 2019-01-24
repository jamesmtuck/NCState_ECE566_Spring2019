%{ 
/* P1. Implements scanner.  Some changes are needed! */

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

typedef struct myvalue {
  int size;
  LLVMValueRef val[32];
} MyValue;

 int line=1;

#include "p1.y.h" 
%}

%option nounput
%option noinput
 
%% 

\n           line++;
[\t ]        ;

def            { return DEF; }

"#"            { return HASH; }

[a-zA-Z_][_a-zA-Z0-9]*  { yylval.id = strdup(yytext); return ID; } 

[0-9]+          { yylval.num = atoi(yytext); return NUM; }

"="	{ return ASSIGN;      } 
";"	{ return SEMI;        } 
"-"	{ return MINUS;       } 
"+"	{ return PLUS;        }  
"*"	{ return MULTIPLY;    } 
"/"	{ return DIVIDE;      } 
","     { return COMMA;       }
"["     { return LBRACKET;    }
"]"     { return RBRACKET;    }

%%
