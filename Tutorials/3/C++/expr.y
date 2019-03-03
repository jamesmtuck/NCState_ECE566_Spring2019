%{

#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include <map>
#include <string>

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

static Function *TheFunction;
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);

int yylex();
int yyerror(const char *);

std::map<std::string,Value*> idMap;

%}

%token ID IMMEDIATE ASSIGN SEMI LPAREN RPAREN LBRACE RBRACE MINUS PLUS MULTIPLY DIVIDE IF WHILE RETURN NOT

%right NOT
%left PLUS MINUS
%left MULTIPLY DIVIDE


%union {
  char *id;
  int imm;
  Value * val;
  BasicBlock *bb;
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
{
// Look to see if we already allocated it
  Value* var = NULL;
  if (idMap.find($1)==idMap.end()) {
     // We haven’t so make a spot on the stack
    var = Builder.CreateAlloca(Builder.getInt64Ty(),nullptr,$1);
     // remember this location and associate it with $1
    idMap[$1] = var;
  } else {
    var = idMap[$1];
  }
  // store $3 into $1’s location in memory
  Builder.CreateStore($3,var);
}
      | IF LPAREN expr RPAREN 
      {
	BasicBlock *then = BasicBlock::Create(TheContext,"if.then",TheFunction);
	BasicBlock *join = BasicBlock::Create(TheContext,"if.join",TheFunction);

	
	Builder.CreateCondBr(Builder.CreateICmpNE($3,Builder.getInt64(0)),then,join);
	Builder.SetInsertPoint(then);
	$<bb>$ = join;
	// now, stmtlist will be put inside then block
      }
        LBRACE stmtlist RBRACE   /*if stmt*/     
      {
	// merge back to join block
	Builder.CreateBr($<bb>5);
	Builder.SetInsertPoint($<bb>5);
      }
| WHILE {
  BasicBlock *expr = BasicBlock::Create(TheContext,"w.expr",TheFunction);
  Builder.CreateBr(expr);
  Builder.SetInsertPoint(expr);
  }
LPAREN expr RPAREN 
{
  BasicBlock *body = BasicBlock::Create(TheContext,"w.body",TheFunction);
  BasicBlock *exit = BasicBlock::Create(TheContext,"w.exit",TheFunction);
  Builder.CreateCondBr(Builder.CreateICmpNE($4, Builder.getInt64(0)),body,exit);
  Builder.SetInsertPoint(body);
  $<bb>$ = exit;
}
LBRACE stmtlist RBRACE 
{
  Builder.CreateBr($<bb>6);
  Builder.SetInsertPoint($<bb>6);	
}
/*while stmt*/
      | SEMI /* null stmt */
;


expr: 
 IMMEDIATE                 
 {
   $$ = Builder.getInt64($1);
  }
| ID
 {
   Value * addr = idMap[$1];
   $$ = Builder.CreateLoad(addr,$1);
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
   $$ = Builder.CreateMul($1,$3);
}  
| expr DIVIDE expr 
{
   $$ = Builder.CreateSDiv($1,$3);
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
  //  $$ = Builder.CreateICmpEQ($2,Builder.getInt64(0));
  Value *icmp = Builder.CreateICmpEQ($2,Builder.getInt64(0));
  $$ = Builder.CreateZExt(icmp,Builder.getInt64Ty());

}
;

%%

int main() {

  // Make Module
  Module *M = new Module("Tutorial3", TheContext);
  
  // Create void function type with no arguments
  FunctionType *FunType = 
    FunctionType::get(Builder.getInt64Ty(),false);
  
  // Create a main function
  TheFunction = Function::Create(FunType,  
					GlobalValue::ExternalLinkage, "main",M);
  
  //Add a basic block to main to hold instructions
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry",
  				      TheFunction);
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
