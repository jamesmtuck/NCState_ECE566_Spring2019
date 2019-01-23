%{
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

int regCnt = 8;

int yylex();
int yyerror(char *);

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

int main() {

  // Make Module
  Module *M = new Module("Tutorial1", TheContext);
  
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

    // Build the return instruction for the function
    Builder.CreateRet(Builder.getInt32(0));
    
    //Write module to file
    std::error_code EC;
    raw_fd_ostream OS("main.bc",EC,sys::fs::F_None);  
    WriteBitcodeToFile(*M,OS);


  } else {
    printf("There was a problem! Read error messages above.\n");
  }

  return 0;
}
