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
LLVMValueRef    Fn;

LLVMBasicBlockRef BBjoin;

%}

%token ID IMMEDIATE ASSIGN SEMI LPAREN RPAREN LBRACE RBRACE MINUS PLUS MULTIPLY DIVIDE IF WHILE RETURN NOT

%right NOT
%left PLUS MINUS
%left MULTIPLY DIVIDE


%union {
  char * id;
  int imm;
  LLVMValueRef val;
  LLVMBasicBlockRef bb;
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

| IF LPAREN expr RPAREN {
  // first mid-rule action

  //  1. Make join block and then block
  LLVMBasicBlockRef then = LLVMAppendBasicBlock(Fn, 
                           "then.block");
  LLVMBasicBlockRef join = LLVMAppendBasicBlock(Fn, 
                           "join.block");

  //  2. Create icmp to evaluate if $3 is true or not:
  LLVMValueRef zero = LLVMConstInt(LLVMTypeOf($3),0,1); 
  LLVMValueRef cond = LLVMBuildICmp(Builder, LLVMIntNE, $3,
                                  zero,"cond");
  //  3. insert conditional branch
  LLVMBuildCondBr(Builder,cond,then,join);
  //  4. position builder in then-block
  LLVMPositionBuilderAtEnd(Builder,then);
  //  5. remember join block somewhere, declare new global 

  $<bb>$ = join; /* declare BBjoin at top of file */
  //  Now stmtlist will be put in the then-block. Excellent!

 } LBRACE stmtlist RBRACE   /*if stmt*/  
 {
   // final action
   //  1. find join block
  LLVMBasicBlockRef join = $<bb>5;
  //  2. insert branch to join block
  LLVMBuildBr(Builder,join);
  //  3. position builder in join block
  LLVMPositionBuilderAtEnd(Builder,join);
  //  done with if
 } 
      | WHILE 
      {
  // 1. Make a block for the cond
  LLVMBasicBlockRef cond = LLVMAppendBasicBlock(Fn,"while.cond");
  // 2. Insert a branch from the current block to cond  
  LLVMBuildBr(Builder,cond);
  // 3. Move the builder to the cond block
  LLVMPositionBuilderAtEnd(Builder, cond);
  // 4. Remember the cond block ($<bb>2)
  $<bb>$ = cond;

      }
LPAREN expr RPAREN 
{
// 1. Make the block for the body
  LLVMBasicBlockRef body = LLVMAppendBasicBlock(Fn,"while.body");
  // 2. Make the join block
  LLVMBasicBlockRef join = LLVMAppendBasicBlock(Fn,"while.join");

  // 3. Build a conditional branch to body and join block  
  LLVMValueRef zero = LLVMConstInt(LLVMTypeOf($4),0,1); 
  LLVMValueRef cond = LLVMBuildICmp(Builder, LLVMIntNE, $4,
                                  zero,"cond");
  LLVMValueRef br = LLVMBuildCondBr(Builder,cond,body,join);
  
  // 4. Position builder in the body
  LLVMPositionBuilderAtEnd(Builder,body);
  // 5. Remember the join block ($<bb>$)
  $<bb>$ = join;


}
LBRACE stmtlist RBRACE 
{
  // 1. Make an unconditional branch back to $<bb>2
  LLVMBuildBr(Builder,$<bb>2);
  // 2. Move builder to $<bb>6
  LLVMPositionBuilderAtEnd(Builder, $<bb>6);
}
/*while stmt*/
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
  Fn = LLVMAddFunction(Module,"main",IntFnTy);

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

