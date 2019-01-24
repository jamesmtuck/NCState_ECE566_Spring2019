%{
#include <stdio.h>
#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"
#include <string.h>

#include "uthash.h"

#include <errno.h>
  //#include <search.h>

extern FILE *yyin;
int yylex(void);
int yyerror(const char *);

extern char *fileNameOut;

extern LLVMModuleRef Module;
extern LLVMContextRef Context;

LLVMValueRef Function;
LLVMBasicBlockRef BasicBlock;
LLVMBuilderRef Builder;

int params_cnt=0;

struct TmpMap{
  char *key;                /* key */
  void *val;                /* data */
  UT_hash_handle hh;        /* makes this structure hashable */
};
 

struct TmpMap *map = NULL;    /* important! initialize to NULL */

void add_val(char *tmp, void* val) { 
  struct TmpMap *s; 
  s = malloc(sizeof(struct TmpMap)); 
  s->key = strdup(tmp); 
  s->val = val; 
  HASH_ADD_KEYPTR( hh, map, s->key, strlen(s->key), s ); 
}

void * get_val(char *tmp) {
  struct TmpMap *s;
  HASH_FIND_STR( map, tmp, s );  /* s: output pointer */
  if (s) 
    return s->val;
  else 
    return NULL; // returns NULL if not found
}

%}

%union {
  char *tmp;
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
    IMPLEMENT: return value, program is over
  */  

  LLVMBuildRet(Builder,LLVMConstInt(LLVMInt64Type(),0,0));
}
;

define: DEF ID LBRACKET NUM RBRACKET SEMI 
{  
  /* NO NEED TO CHANGE ANYTHING IN THIS RULE */

  /* Now we know how many parameters we need.  Create a function type
     and add it to the Module */

  params_cnt = $4;

  LLVMTypeRef Integer = LLVMInt64TypeInContext(Context);

  LLVMTypeRef *IntRefArray = malloc(sizeof(LLVMTypeRef)*params_cnt);
  int i;
  
  /* Build type for function */
  for(i=0; i<params_cnt; i++)
    IntRefArray[i] = Integer;

  LLVMBool var_arg = 0; /* false */
  LLVMTypeRef FunType = LLVMFunctionType(Integer,IntRefArray,params_cnt,var_arg);

  /* Found in LLVM-C -> Core -> Modules */
  Function = LLVMAddFunction(Module,$2,FunType);

  /* Add a new entry basic block to the function */
  BasicBlock = LLVMAppendBasicBlock(Function,"entry");

  /* Create an instruction builder class */
  Builder = LLVMCreateBuilder();

  /* Insert new instruction at the end of entry block */
  LLVMPositionBuilderAtEnd(Builder,BasicBlock);
}
;

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

int line;

int yyerror(const char *msg)
{
  printf("%s at line %d.\n",msg,line);
  return 0;
}
