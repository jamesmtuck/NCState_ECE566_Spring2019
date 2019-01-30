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
int lastReg = 0;

int yylex();
int yyerror(const char *);

Value * regs[8];

%}

%token REG IMMEDIATE ASSIGN SEMI LPAREN RPAREN LBRACKET RBRACKET MINUS PLUS

%left PLUS MINUS

%union {
  int reg;
  int imm;
  Value * val;
}

%type <reg> REG 
%type <imm> IMMEDIATE
%type <val> expr

%start program


%%

program: REG ASSIGN expr SEMI 
{
  //printf("# Output program : REG (%d) ASSIGN expr SEMI\n", $1);
  //printf("ADD R%d, R%d, 0\n", $1, $3);
  regs[$1] = $3;
  lastReg = $1;
} 
| program REG ASSIGN expr SEMI
{
  //printf("# Output program : REG (%d) ASSIGN expr SEMI\n", $2);
  //printf("ADD R%d, R%d, 0\n", $2, $4);
  regs[$2] = $4;
  lastReg = $2;
} 

| program SEMI
{
  Builder.CreateRet(regs[lastReg]);
  return 0;
}
;
expr: 
 IMMEDIATE                 
 {
   //int reg = regCnt++;
   ///printf("# Output for expr : IMMEDIATE (%d) \n", $1);
   //printf("AND R%d, R%d, 0\n", reg, reg);
   //printf("ADD R%d, R%d, %d\n", reg, reg, $1);
   $$ = Builder.getInt32($1);
   
   ConstantInt *ci = dyn_cast<ConstantInt*>($$);
   if (ci != NULL) {
     printf("%lld\n", ci->getZExtValue());
   }

 }
| REG
 {
   //printf("# Output expr : REG (%d) \n",$1);
   //$$ = $1;
   $$ = regs[$1];
 }
| expr PLUS expr  
 {
   /*printf("# Output expr : expr PLUS expr \n");
  int reg = regCnt++;
  printf("ADD R%d, R%d, R%d\n", reg, $1, $3);
  $$ = reg;*/
   $$ = Builder.CreateAdd($1,$3);
 }

| expr MINUS expr 
 {
   /* printf("# Output for expr : expr MINUS expr \n");

  int reg = regCnt++;
  printf("SUB R%d, R%d, R%d\n", reg, $1, $3);
  $$ = reg;*/  
   $$ = Builder.CreateSub($1,$3);
}


| LPAREN expr RPAREN 
 {
   /*printf("# Output for expr : LPAREN expr RPAREN \n");
     $$ = $2;*/
   $$ = $2;
 }

| MINUS expr 
 {
   /*printf("expr : MINUS expr \n");
  
  int reg = regCnt++;
  printf("NOT R%d, R%d\n", reg, $2);
  printf("ADD R%d, R%d, 1\n", reg, reg);
  $$ = reg;  */
      $$ = Builder.CreateNeg($2);
 }

| LBRACKET expr RBRACKET 
 {
   /*printf("#Output for expr : LBRACKET expr RBRACKET \n");

  int reg = regCnt++;
  printf("LDR R%d, R%d, 0\n", reg, $2);
  $$ = reg;*/  
   Value * tmp = Builder.CreateIntToPtr($2,   
	      PointerType::get(Builder.getInt32Ty(),0));

   $$ = Builder.CreateLoad(tmp);
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
    //Builder.CreateRet(Builder.getInt32(0));
    
    //M->dump();

    //Write module to file
    std::error_code EC;
    raw_fd_ostream OS("main.bc",EC,sys::fs::F_None);  
    WriteBitcodeToFile(*M,OS);

  } else {
    printf("There was a problem! Read error messages above.\n");
  }

  return 0;
}
