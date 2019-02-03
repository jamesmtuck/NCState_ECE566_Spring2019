%{
#include <stdio.h>

int regCnt=10;

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

void yyerror(const char*);
int yylex();

LLVMBuilderRef  Builder;
LLVMValueRef regs[8] = {NULL};

%}

%token REG IMMEDIATE ASSIGN SEMI LPAREN RPAREN LBRACKET RBRACKET MINUS PLUS

%left PLUS MINUS

%union {
  int reg;
  int imm;
  LLVMValueRef val;
}

%type <reg> REG
%type <imm> IMMEDIATE
%type <val> expr program

%%

program: REG ASSIGN expr SEMI 
{
  //printf("#REG (%d) ASSIGN expr SEMI\n", $1);
  //printf("ADD R%d, R%d, 0\n",$1, $3);
  regs[$1] = $3;
  $$ = $3;

}
| program REG ASSIGN expr SEMI 
{ 
  regs[$2] = $4;
  $$ = $4;
}
| program SEMI          
   { 
  LLVMBuildRet(Builder,$1);  
  return 0; 
 }
;

expr: 

 IMMEDIATE                 
 {
   //printf("#IMMEDIATE (%d)\n",$1);
   //printf("SET R%d, %d\n",regCnt,$1);
   //$$ = regCnt++;
    $$ = LLVMConstInt(LLVMInt32Type(),$1,0);
 }
| REG
{
  //printf("#REG (%d)\n",$1);
  $$ = regs[$1];
}  
| expr PLUS expr  
{
  //printf("# expr (%d) PLUS expr (%d)\n",$1,$3);
  //printf("ADD R%d, R%d, R%d\n", regCnt, $1, $3);
  //$$ = regCnt++;
  $$ = LLVMBuildAdd(Builder,$1,$3,"");
}  
| expr MINUS expr 
{
  //printf("#expr MINUS expr\n");
  //printf("SUB R%d, R%d, R%d\n", regCnt, $1, $3);
  //$$ = regCnt++;
  $$ = LLVMBuildSub(Builder,$1,$3,"");
}  

| LPAREN expr RPAREN 
{
  //printf("LPAREN expr RPAREN\n");
  $$ = $2;
}  

| MINUS expr 
{
  //printf("MINUS expr\n");
  //printf("NOT R%d, R%d\n", regCnt, $2);
  //printf("ADD R%d, R%d, 1", regCnt, regCnt);
  //$$ = regCnt++;
  $$ = LLVMBuildNeg(Builder,$2,"neg");
}  

| LBRACKET expr RBRACKET 
{
  //printf("LBRACKET expr RBRACKET\n");
  //printf("LDR R%d, R%d, 1", regCnt, $2);
  //$$ = regCnt++;
  LLVMValueRef tmp = LLVMBuildIntToPtr(Builder, $2, LLVMPointerType(LLVMInt32Type(),0),"inttoptr");
  $$ = LLVMBuildLoad(Builder,tmp, "load");
}  

;


%%

int main() {

// Make a Module
  LLVMModuleRef Module = LLVMModuleCreateWithName("main");
  
  // Make a void function type with no arguments
  LLVMTypeRef IntFnTy = LLVMFunctionType(LLVMInt32Type(),NULL,0,0);
  
  // Make a void function named main (the start of the program!)
  LLVMValueRef Fn = LLVMAddFunction(Module,"main",IntFnTy);

  // Add a basic block to main to hold new instructions
  LLVMBasicBlockRef BB = LLVMAppendBasicBlock(Fn,"entry");
  
  // Create a Builder object that will construct IR for us
  Builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(Builder,BB);

  // Now we’re ready to make IR, call yyparse()

  // yyparse() triggers parsing of the input
  if (yyparse()==0) {
  //LLVMBuildRet(Builder,LLVMConstInt(LLVMInt32Type(),0,0));
     LLVMWriteBitcodeToFile(Module,"main.bc");
    // all is good
  } else {
    printf("There was a problem! Read error messages above.\n");
  }
  return 0;
}

