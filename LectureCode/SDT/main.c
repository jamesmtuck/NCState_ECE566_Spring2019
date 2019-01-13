#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

extern FILE *yyin;
char *fileNameOut;
LLVMModuleRef Module;
LLVMContextRef Context;
LLVMValueRef Function;
LLVMBuilderRef Builder;

extern void initialize();

extern void yyparse();

int
main (int argc, char ** argv)
{
  /* ./calc */
  if (argc < 2)
    {      
      printf("Usage: calc out.bc\n");
      return 0;
    }

  /* create an LLVMContext for use when generating code */
  Context = LLVMContextCreate();
 
  /* create an LLVM Module to hold the function */
  Module = LLVMModuleCreateWithNameInContext("main",Context);

  /* this is the name of the file to generate, you can also use
     this string to figure out the name of the generated function */
  fileNameOut = strdup(argv[1]);

  LLVMTypeRef Integer = LLVMInt64TypeInContext(Context);

  LLVMTypeRef FunType = LLVMFunctionType(Integer,0,0,0);

  Function = LLVMAddFunction(Module,"main",FunType);
  
  LLVMBasicBlockRef BasicBlock = LLVMAppendBasicBlock(Function,"entry");

  Builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(Builder,BasicBlock);

  yyin = stdin;
  yyparse();

  /* Write bitcode to file with name argv[1] */
  LLVMWriteBitcodeToFile(Module,argv[1]);

  return 0;
}
