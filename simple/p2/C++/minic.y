%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
//#include "llvm/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"


#include <memory>
#include <algorithm>

#include "list.h"
#include "symbol.h"

using namespace llvm;

int num_errors;

extern int yylex();   /* lexical analyzer generated from lex.l */

int yyerror(const char *error);
int parser_error(const char*);

void minic_abort();
char *get_filename();
int get_lineno();

int loops_found=0;

extern Module *M;

Function *Fun;
IRBuilder<> *Builder;

Value* BuildFunction(Type* RetType, const char *name, 
			   paramlist_t *params);

%}

/* Data structure for tree nodes*/

%union {
  int inum;
  int fnum;
  char * id;
  Type*  type;
  Value* value;
  BasicBlock *bb;
  paramlist_t *params;
}

/* these tokens are simply their corresponding int values, more terminals*/

%token SEMICOLON COMMA COLON
%token LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET
%token ASSIGN PLUS MINUS STAR DIV MOD 
%token LT GT LTE GTE EQ NEQ NOT
%token LOGICAL_AND LOGICAL_OR
%token BITWISE_OR BITWISE_XOR LSHIFT RSHIFT BITWISE_INVERT

%token DOT ARROW AMPERSAND QUESTION_MARK

%token FOR WHILE IF ELSE DO STRUCT SIZEOF RETURN SWITCH
%token BREAK CONTINUE CASE
%token INT VOID FLOAT

/* no meaning, just placeholders */
%token STATIC AUTO EXTERN TYPEDEF CONST VOLATILE ENUM UNION REGISTER
/* NUMBER and ID have values associated with them returned from lex*/

%token <inum> CONSTANT_INTEGER /*data type of NUMBER is num union*/
%token <fnum> CONSTANT_FLOAT /*data type of NUMBER is num union*/
%token <id>  ID

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/* values created by parser*/

%type <id> declarator
%type <params> param_list param_list_opt
%type <value> expression
%type <value> assignment_expression
%type <value> conditional_expression
%type <value> constant_expression
%type <value> logical_OR_expression
%type <value> logical_AND_expression
%type <value> inclusive_OR_expression
%type <value> exclusive_OR_expression
%type <value> AND_expression
%type <value> equality_expression
%type <value> relational_expression
%type <value> shift_expression
%type <value> additive_expression
%type <value> multiplicative_expression
%type <value> cast_expression
%type <value> unary_expression
%type <value> lhs_expression
%type <value> postfix_expression
%type <value> primary_expression
%type <value> constant
%type <type> type_specifier
%type <value> opt_initializer
/* 
   The grammar used here is largely borrowed from Kernighan and Ritchie's "The C
   Programming Language," 2nd Edition, Prentice Hall, 1988. 

   But, some modifications have been made specifically for MiniC!
 */

%%

/* 
   Beginning of grammar: Production Rules
   when you want to use the value returned by a token from a lower part of the tree you refer to it as $X, X is the position
   in the expression it is located in ie in this grammar "|type_specifier pointer direct_decl LPAREN param_list_opt RPAREN " 
   if you want to use the value returned by direct_decl you can refer to it as "$3" when you want to return a value you  
   place set $$ = to it. 
*/

translation_unit:	  external_declaration
			| translation_unit external_declaration
;

external_declaration:	  function_definition
{
  /* finish compiling function */
  if(num_errors>100)
    {
      minic_abort();
    }
  else if(num_errors==0)
    {
      
    }
}
                        | declaration 
{ 
  /* nothing to be done here */
}
;

function_definition:	  type_specifier ID LPAREN param_list_opt RPAREN 
{
  symbol_push_scope();
  /* This is a mid-rule action */
  BuildFunction($1,$2,$4);  
} 
                          compound_stmt 
{ 
  /* FIX THIS HACK */
  BasicBlock *BB = Builder->GetInsertBlock();
  if(!BB->getTerminator())
    {
      Builder->CreateRet(Builder->getInt32(0));
    }

  symbol_pop_scope();
  /* make sure basic block has a terminator (a return statement) */
}
                        | type_specifier STAR ID LPAREN param_list_opt RPAREN 
{
  symbol_push_scope();
  BuildFunction(PointerType::get($1,0),$3,$5);
} 
                          compound_stmt 
{ 
  /* This is the rule completion */


  /* make sure basic block has a terminator (a return statement) */

  symbol_pop_scope();
}
;

declaration:    type_specifier STAR ID opt_initializer SEMICOLON
{
  if (is_global_scope())
    {
      Twine name($3);
      new GlobalVariable(*M,(Type*)PointerType::get($1,0),false,GlobalValue::ExternalLinkage,(Constant*)NULL,name);
    } 
  else
    {
      symbol_insert($3,  /* map name to alloca */
		    Builder->CreateAlloca(PointerType::get($1,0),NULL,$3)); /* build alloca */
    }

} 
              | type_specifier ID opt_initializer SEMICOLON
{
  if (is_global_scope())
    {
      Twine name($2);
      new GlobalVariable(*M,(Type*)$1,false,GlobalValue::ExternalLinkage,(Constant*)NULL,name);
    }
  else
    {
      symbol_insert($2,  /* map name to alloca */
		    Builder->CreateAlloca($1,NULL,$2)); /* build alloca */
    }
} 
;

declaration_list:	   declaration
{

}
                         | declaration_list declaration  
{

}
;


type_specifier:		  INT 
{
  $$ = IntegerType::get(M->getContext(),32);
}
;


declarator:		  ID
{
  $$ = $1;
}
;

opt_initializer: ASSIGN constant_expression	      
{
  $$ = $2;
}
| // nothing
{
  // indicate there is none
  $$ = NULL;
}
;


param_list_opt:           
{ 
  $$ = NULL;
}
                        | param_list
{ 
  $$ = $1;
}
;

param_list:	
			  param_list COMMA type_specifier declarator
{
  $$ = push_param($1,$4,$3);
}
			| param_list COMMA type_specifier STAR declarator
{
  $$ = push_param($1,$5,(Type*)PointerType::get($3,0));
}
                        | param_list COMMA type_specifier
{
  $$ = push_param($1,NULL,$3);
}
			|  type_specifier declarator
{
  /* create a parameter list with this as the first entry */
  $$ = push_param(NULL, $2, $1);
}
			| type_specifier STAR declarator
{
  /* create a parameter list with this as the first entry */
  $$ = push_param(NULL, $3, (Type*)PointerType::get($1,0));
}
                        | type_specifier
{
  /* create a parameter list with this as the first entry */
  $$ = push_param(NULL, NULL, $1);
}
;


statement:		  expr_stmt            
			| compound_stmt        
			| selection_stmt       
			| iteration_stmt       
			| jump_stmt            
                        | break_stmt
                        | continue_stmt
                        | case_stmt
;

expr_stmt:	           SEMICOLON            
{ 

}
			|  expression SEMICOLON       
{ 

}
;

compound_stmt:		  LBRACE declaration_list_opt statement_list_opt RBRACE 
{

}
;

declaration_list_opt:	
{

}
			| declaration_list
{

}
;

statement_list_opt:	
{

}
			| statement_list
{

}
;

statement_list:		statement
{

}
			| statement_list statement
{

}
;

break_stmt:               BREAK SEMICOLON
{

};

case_stmt:                CASE constant_expression COLON
{
  // BONUS: part of switch implementation
};

continue_stmt:            CONTINUE SEMICOLON
{

};

selection_stmt:		  
		          IF LPAREN expression RPAREN statement ELSE statement 
{ 

}
| 		          SWITCH LPAREN expression RPAREN statement 
{
  // +10 BONUS POINTS for a fully correct implementation
}
;


iteration_stmt:		  
WHILE LPAREN { 


} expression RPAREN { 
  // create new body and exit blocks

  // call push loop to record this loop's important blocks
  //push_loop(expr,body,body,after);
} 
  statement
{
  //loop_info_t info = get_loop();

  // insert back edge from body to header

  //pop_loop();
}

| FOR LPAREN expr_opt SEMICOLON expr_opt SEMICOLON expr_opt RPAREN statement
{
  // 566: add mid-rule actions to support for-statement
}
;

expr_opt:		
{ 

}
			| expression
{ 

}
;

jump_stmt:		  RETURN SEMICOLON
{ 
  Builder->CreateRetVoid();

}
			| RETURN expression SEMICOLON
{
  Builder->CreateRet($2);
}
;

expression:               assignment_expression
{ 
  $$=$1;
}
;

assignment_expression:    conditional_expression
{
  $$=$1;
}
                        | lhs_expression ASSIGN assignment_expression
{
  // build store using address from lhs_expression
}
;

constant_expression:       conditional_expression
{ $$ = $1; }
;

conditional_expression:   logical_OR_expression
{
  $$=$1;
}
                        | logical_OR_expression QUESTION_MARK expression COLON conditional_expression
{
  // Hint: select instruction
}
;

logical_OR_expression:    logical_AND_expression
{
  $$ = $1;
}
                        | logical_OR_expression LOGICAL_OR logical_AND_expression
{
  // Hint: do not build control flow, build an 2 icmps and 2 selects and one or
  // Since we do not need to enforce short circuiting, you can take some short cuts!

};

logical_AND_expression:   inclusive_OR_expression
{
  $$ = $1;
}
                        | logical_AND_expression LOGICAL_AND inclusive_OR_expression
{
  // Just like LOGICAL_OR above but with LOGICAL_AND
}
;

inclusive_OR_expression:  exclusive_OR_expression
{
    $$=$1;
}
                        | inclusive_OR_expression BITWISE_OR exclusive_OR_expression
{
  // Implement
}
;

exclusive_OR_expression:  AND_expression
{
  $$ = $1;
}
                        | exclusive_OR_expression BITWISE_XOR AND_expression
{
  // Implement
}
;

AND_expression:           equality_expression
{
  $$ = $1;
}
                        | AND_expression AMPERSAND equality_expression
{
  // Implement
}
;

equality_expression:      relational_expression
{
  $$ = $1;
}
                        | equality_expression EQ relational_expression
{
  // Implement
}
                        | equality_expression NEQ relational_expression
{
  // Implement
}
;

relational_expression:    shift_expression
{
    $$=$1;
}
                        | relational_expression LT shift_expression
{
  // Implement: icmp
}
                        | relational_expression GT shift_expression
{
  // Implement: icmp
}
                        | relational_expression LTE shift_expression
{
  // Implement
}
                        | relational_expression GTE shift_expression
{
  // Implement
}
;

shift_expression:         additive_expression
{
    $$=$1;
}
                        | shift_expression LSHIFT additive_expression
{
  // Implement
}
                        | shift_expression RSHIFT additive_expression
{
  // Implement
}
;

additive_expression:      multiplicative_expression
{
  $$ = $1;
}
                        | additive_expression PLUS multiplicative_expression
{
  // Implement
}
                        | additive_expression MINUS multiplicative_expression
{
  // Implement
}
;

multiplicative_expression:  cast_expression
{
  $$ = $1;
}
                        | multiplicative_expression STAR cast_expression
{
  // Implement
}
                        | multiplicative_expression DIV cast_expression
{
  // Implement
}
                        | multiplicative_expression MOD cast_expression
{
  // Implement
}
;

cast_expression:          unary_expression
{ 
  $$ = $1; 
}
                        | LPAREN type_specifier RPAREN cast_expression
{
  // Implement 
  $$ = $4; 
}
                        | LPAREN type_specifier STAR RPAREN cast_expression
{ 
  // Implement
  $$ = $5; 
}

;

lhs_expression:           ID 
{
  Value* val = symbol_find($1);
  if (val==NULL)
    {
      // error
    }
  else
    $$ = val;
}
                        | STAR ID
{
  Value* val = symbol_find($2);
  if (val==NULL)
    {
      // error
    }
  else
    $$ = Builder->CreateLoad(val);
}
;

unary_expression:         postfix_expression
{
  $$ = $1;
}
                        | AMPERSAND primary_expression
{
  // Implement
  $$ = $2;
}
                        | STAR primary_expression
{
  $$ = $2;
  // Implement
}
                        | MINUS unary_expression
{
  // Implement
}
                        | PLUS unary_expression
{
  $$ = $2;
}
                        | BITWISE_INVERT unary_expression
{
  // Implement: Hint: xor
}
                        | NOT unary_expression
{
  // Implement
}
;


postfix_expression:       primary_expression
{
  $$ = $1;
}
;

primary_expression:       ID 
{ 
  Value* val = symbol_find($1);
  if (val==NULL) {
    // Handle error
  } else
    $$ = Builder->CreateLoad(val);
}
                        | constant
{
  $$ = $1;
}
                        | LPAREN expression RPAREN
{
  $$ = $2;
}
;

constant:	          CONSTANT_INTEGER  
{ 
  // Implement
} 
|                         CONSTANT_FLOAT
{
  // Implement
}
;

%%

Value* BuildFunction(Type* RetType, const char *name, 
			   paramlist_t *params)
{
  int i;
  int size = paramlist_size(params);

  std::vector<Type*> v;
  std::vector<const char*> vname;

  v.resize(size,NULL);
  vname.resize(size,NULL);

  paramlist_t *tmp = params;
  i=size-1;
  while(tmp)
    {
      v[i]=(tmp->type);
      vname[i]=(tmp->name);
      tmp = next_param(tmp);
      i--;
    }
  ArrayRef<Type*> Params(v);

  FunctionType* FunType = FunctionType::get(RetType,Params,false);

  Fun = Function::Create(FunType,GlobalValue::ExternalLinkage,
			 name,M);
  Twine T("entry");
  BasicBlock *BB = BasicBlock::Create(M->getContext(),T,Fun);

  /* Create an Instruction Builder */
  Builder = new IRBuilder<>(M->getContext());
  Builder->SetInsertPoint(BB);

  Function::arg_iterator I = Fun->arg_begin();
  for(i=0; I!=Fun->arg_end();i++, I++)
    {
      // map args and create allocas!
      AllocaInst *AI = Builder->CreateAlloca(v[i]);
      Builder->CreateStore(&(*I),(Value*)AI);
      symbol_insert(vname[i],(Value*)AI);
    }


  return Fun;
}

extern int line_num;
extern char *infile[];
static int   infile_cnt=0;
extern FILE * yyin;

int parser_error(const char *msg)
{
  printf("%s (%d): Error -- %s\n",infile[infile_cnt-1],line_num,msg);
  return 1;
}

int internal_error(const char *msg)
{
  printf("%s (%d): Internal Error -- %s\n",infile[infile_cnt-1],line_num,msg);
  return 1;
}

int yywrap() {
  static FILE * currentFile = NULL;

  if ( (currentFile != 0) ) {
    fclose(yyin);
  }
  
  if(infile[infile_cnt]==NULL)
    return 1;

  currentFile = fopen(infile[infile_cnt],"r");
  if(currentFile!=NULL)
    yyin = currentFile;
  else
    printf("Could not open file: %s",infile[infile_cnt]);

  infile_cnt++;
  
  return (currentFile)?0:1;
}

int yyerror(const char* error)
{
  parser_error("Un-resolved syntax error.");
  return 1;
}

char * get_filename()
{
  return infile[infile_cnt-1];
}

int get_lineno()
{
  return line_num;
}


void minic_abort()
{
  parser_error("Too many errors to continue.");
  exit(1);
}
