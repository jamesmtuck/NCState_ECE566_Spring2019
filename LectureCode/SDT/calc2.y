%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "llvm-c/Core.h"

extern FILE *yyin;

 int yyerror(const char*);
 int yylex();

 extern LLVMBuilderRef Builder;
 extern LLVMContextRef Context;
%}

%union {
  int num;
  LLVMValueRef val;
}

%token NUMBER PLUS MINUS LPAREN RPAREN NEWLINE DONE

%type <val> expr expr_list
%type <num> NUMBER

%left PLUS MINUS

%start program

%%

program:   expr_list DONE             
{    
     LLVMBuildRet(Builder, 
		 $1);
     return 0;          
}
;

expr_list:   
expr NEWLINE              {$$ = $1;}
| expr_list expr NEWLINE  {$$ = $2;}
;

expr :   expr PLUS expr               {$$ = LLVMBuildAdd(Builder,$1,$3,""); }
       | expr MINUS expr              {$$ = LLVMBuildSub(Builder,$1,$3,"");                     } 
| LPAREN expr RPAREN                  { $$ = $2; }
       | NUMBER                       {

	 // allocate space on the stack
	 LLVMValueRef alloca = LLVMBuildAlloca(Builder,LLVMInt64TypeInContext(Context),"");
	 
	 // store the constant to the stack
	 LLVMBuildStore(Builder,LLVMConstInt( LLVMInt64TypeInContext(Context),(unsigned long long int)$1,(LLVMBool)1 ),alloca);

	 // load the constant
	 $$ = LLVMBuildLoad(Builder,alloca,"");

}
| MINUS expr                          {$$ = LLVMBuildNeg(Builder,$2,""); }
;

%%

int yyerror(const char *msg) {
  printf("%s",msg);
  return 0;
}

//int main(){
//	yyin = stdin ; 
//	yyparse() ;
//	return 0;
//}
