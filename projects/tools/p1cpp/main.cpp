#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"

#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;

extern FILE *yyin;
char *fileNameOut;
Module* M;
LLVMContext TheContext;

void initialize();
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

  M = new Module("main", TheContext);

  /* this is the name of the file to generate, you can also use
     this string to figure out the name of the generated function */
  fileNameOut = strdup(argv[2]);

  // Set input to specific file
  yyin = fopen(argv[1],"r");
  
  if (yyparse()==0)
    {
      /* Write bitcode to file with name argv[2] */
  
      //Print the module to file
      std::error_code EC;
      raw_fd_ostream OS(fileNameOut,EC,sys::fs::F_None);  
      WriteBitcodeToFile(*M,OS);
      return 0;
    }
  else 
    {
      printf("Error parsing file!\n");
      return 1;
    }

  return 0;
}
