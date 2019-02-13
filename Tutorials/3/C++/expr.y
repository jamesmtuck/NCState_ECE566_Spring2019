%{

#include <cstdlib>
#include <iostream>
#include <stdio.h>
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

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);

int yylex();
int yyerror(const char *);

%}

%token ID IMMEDIATE ASSIGN SEMI LPAREN RPAREN LBRACE RBRACE MINUS PLUS MULTIPLY DIVIDE IF WHILE RETURN NOT

%right NOT
%left PLUS MINUS
%left MULTIPLY DIVIDE


%union {
  char *id;
  int imm;
  Value * val;
}

%type <id> ID
%type <imm> IMMEDIATE
%type <val> expr

%start program


%%

program: LBRACE stmtlist RETURN expr SEMI RBRACE
{
  Builder.CreateRet($4);
  return 0;
}
;

stmtlist :    stmt
           |  stmtlist stmt

;

stmt:   ID ASSIGN expr SEMI              /* expression stmt */
      | IF LPAREN expr RPAREN LBRACE stmtlist RBRACE   /*if stmt*/     
      | WHILE LPAREN expr RPAREN LBRACE stmtlist RBRACE /*while stmt*/
      | SEMI /* null stmt */
;


expr: 
 IMMEDIATE                 
 {
   $$ = Builder.getInt32($1);
 }
| ID
 {

 }
| expr PLUS expr  
 {
   $$ = Builder.CreateAdd($1,$3);
 }

| expr MINUS expr 
 {
   $$ = Builder.CreateSub($1,$3);
}

| expr MULTIPLY expr  
{

}  
| expr DIVIDE expr 
{

}  

| LPAREN expr RPAREN 
 {
   $$ = $2;
 }

| MINUS expr 
 {
   $$ = Builder.CreateNeg($2);
 }

| NOT expr
{

}
;

%%

int main() {

  // Make Module
  Module *M = new Module("Tutorial3", TheContext);
  
  // Create void function type with no arguments
  FunctionType *FunType = 
    FunctionType::get(Builder.getInt32Ty(),false);
  
  // Create a main function
  Function *Function = Function::Create(FunType,  
					GlobalValue::ExternalLinkage, "main",M);
  
  //Add a basic block to main to hold instructions
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry",
				      Function);
  // Ask builder to place new instructions at end of the
  // basic block
  Builder.SetInsertPoint(BB);
  
  // Now we’re ready to make IR, call yyparse()
  
  // yyparse() triggers parsing of the input
  if (yyparse()==0) {
    // all is good

    M->print(errs(),nullptr);

    //Write module to file
    std::error_code EC;
    raw_fd_ostream OS("main.bc",EC,sys::fs::F_None);  
    WriteBitcodeToFile(*M,OS);

  } else {
    printf("There was a problem! Read error messages above.\n");
  }

  return 0;
}
