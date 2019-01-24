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

extern void initialize();
int yyparse();

int
main (int argc, char ** argv)
{
  /* ./p1 filein fileout */

  if (argc < 3) {
    fprintf(stderr,"Not enough positional arguments to %s.\n",argv[0]);
    fprintf(stderr,"usage: %s filein.p1 fileout.bc\n",argv[0]);
    return 1;
  }

  /* initialize parser related data if you need to */
  initialize();

  /* create an LLVMContext for use when generating code */
  Context = LLVMGetGlobalContext();

  /* create an LLVM Module to hold the function */
  Module = LLVMModuleCreateWithNameInContext("main",Context);

  /* this is the name of the file to generate, you can also use
     this string to figure out the name of the generated function */
  fileNameOut = strdup(argv[2]);

  // Set input to specific file
  yyin = fopen(argv[1],"r");
  
  if (yyparse()==0)
    {
      /* Write bitcode to file with name argv[2] */
      LLVMWriteBitcodeToFile(Module,argv[2]);     
      return 0;
    }
  else 
    {
      printf("Error parsing file!\n");
      return 1;
    }

  return 0;
}
