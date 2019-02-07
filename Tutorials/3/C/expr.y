%{
#include <stdio.h>
#include "uthash.h"
#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

struct IDMap{
  char *key;                /* key */
  void *val;                /* data */
  UT_hash_handle hh;        /* makes this structure hashable */
};
 
struct IDMap *map = NULL;    /* important! initialize to NULL */

void map_insert(char *key, void* val) { 
  struct IDMap *s; 
  s = malloc(sizeof(struct IDMap)); 
  s->key = strdup(key); 
  s->val = val; 
  HASH_ADD_KEYPTR( hh, map, s->key, strlen(s->key), s ); 
}

void * map_find(char *tmp) {
  struct IDMap *s;
  HASH_FIND_STR( map, tmp, s );  /* s: output pointer */
  if (s) 
    return s->val;
  else 
    return NULL; // returns NULL if not found
}

void yyerror(const char*);
int yylex();

LLVMBuilderRef  Builder;

%}

%token ID IMMEDIATE ASSIGN SEMI LPAREN RPAREN LBRACE RBRACE MINUS PLUS MULTIPLY DIVIDE IF WHILE RETURN NOT

%right NOT
%left PLUS MINUS
%left MULTIPLY DIVIDE


%union {
  char * id;
  int imm;
  LLVMValueRef val;
}

%type <id> ID
%type <imm> IMMEDIATE
%type <val> expr  

%%


program : LBRACE stmtlist RETURN expr SEMI RBRACE
{
  LLVMBuildRet(Builder,$4);
  return 0;
}
;

stmtlist :    stmt
           |  stmtlist stmt

;

stmt:   ID ASSIGN expr SEMI              /* expression stmt */
{
  // Look to see if we already allocated it
  LLVMValueRef var = map_find($1);
  if (var==NULL) {
     // We haven’t so make a spot on the stack
     var = LLVMBuildAlloca(Builder,LLVMInt64Type(),$1);
     // remember this location and associate it with $1
     map_insert($1,var);
  }
  // store $3 into $1’s location in memory
  LLVMBuildStore(Builder,$3,var);
}

      | IF LPAREN expr RPAREN LBRACE stmtlist RBRACE   /*if stmt*/     
      | WHILE LPAREN expr RPAREN LBRACE stmtlist RBRACE /*while stmt*/
      | SEMI /* null stmt */
;


expr: 

 IMMEDIATE                 
 {
    $$ = LLVMConstInt(LLVMInt64Type(),$1,0);
 }
| ID
{
  LLVMValueRef alloca = map_find($1);
  $$ = LLVMBuildLoad(Builder,alloca,$1);
}  
| expr PLUS expr  
{
  $$ = LLVMBuildAdd(Builder,$1,$3,"");
}  
| expr MINUS expr 
{
  $$ = LLVMBuildSub(Builder,$1,$3,"");
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
  $$ = LLVMBuildNeg(Builder,$2,"neg");
}  
| NOT expr
{
  LLVMValueRef zero = LLVMConstInt(LLVMTypeOf($2),0,1); 
  LLVMValueRef icmp = LLVMBuildICmp(Builder, LLVMIntEQ, $2,
                                  zero,"logical.not");
  //$$ = LLVMBuildSelect(Builder, 
  //		       icmp, // condition 
  //                   LLVMConstInt(LLVMInt64Type(),1,1),   // if-true
  //LLVMConstInt(LLVMInt64Type(), 0, 1), // if-false
  //"logical.not");
  
  $$ = LLVMBuildZExt(Builder, icmp, LLVMInt64Type(),"logical.not");
}
;


%%

int main() {

// Make a Module
  LLVMModuleRef Module = LLVMModuleCreateWithName("Tutorial3");
  
  // Make a void function type with no arguments
  LLVMTypeRef IntFnTy = LLVMFunctionType(LLVMInt64Type(),NULL,0,0);
  
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
    
    LLVMDumpModule(Module);
    
    LLVMWriteBitcodeToFile(Module,"main.bc");
    // all is good
  } else {
    printf("There was a problem! Read error messages above.\n");
  }
  return 0;
}

