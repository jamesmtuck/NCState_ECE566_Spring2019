%{
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"

#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;

extern FILE *yyin;
int yylex(void);
int yyerror(const char *);

extern char *fileNameOut;

extern Module *M;
extern LLVMContext TheContext;

IRBuilder<> Builder(TheContext);

int params_cnt=0;

%}

%union {
  int num;
  char *id;
}

%token DEF ASSIGN SEMI COMMA MINUS PLUS LBRACKET RBRACKET SUM TMP NUM ID RAISE HASH 
%type <num> NUM 
%type <id> ID


//%nonassoc QUESTION COLON
%left PLUS MINUS
%left MULTIPLY DIVIDE
%left RAISE

%start program

%%

program: define stmtlist 
{ 
  /* 
    IMPLEMENT: return value
    Hint: the following code is not sufficient
  */    
  Builder.CreateRet(Builder.getInt64(0));
  return 0;
}
;

define: DEF ID LBRACKET NUM RBRACKET SEMI 
{  
  /* NO NEED TO CHANGE ANYTHING IN THIS RULE BUT YOU CAN IF YOU WANT */

  params_cnt = $4;

  std::vector<Type*> params;
  for(int i=0; i<params_cnt; i++)
    params.push_back(Builder.getInt64Ty());

  FunctionType *FunType = FunctionType::get(Builder.getInt64Ty(),params,false);

  // Create a main function
  Function *Func = Function::Create(FunType,GlobalValue::ExternalLinkage,$2,M);
  
  //Add a basic block to the function
  BasicBlock *BB = BasicBlock::Create(TheContext,"entry",Func);

  Builder.SetInsertPoint(BB);
}
;

/*
   IMPLMENT ALL THE RULES BELOW HERE!
 */

stmtlist:  stmtlist stmt 
| stmt                   
;         

stmt: ID ASSIGN expr SEMI;


expr:   expr MINUS expr
      | expr PLUS expr
      | expr RAISE expr
      | MINUS expr 
      | expr MULTIPLY expr
      | expr DIVIDE expr
      | LBRACKET expr RBRACKET
      | ID
      | NUM
      | HASH
;

%%


void initialize()
{
  /* IMPLEMENT: add something here if needed */
}

extern int line;

int yyerror(const char *msg)
{
  printf("%s at line %d.\n",msg,line);
  return 0;
}
