#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"
#include "llvm-c/Analysis.h"
#include "worklist.h"

void NoOptimization(LLVMModuleRef Module) 
{

}

int isDead(LLVMValueRef I)
{
  // All conditions passed
  return 0;
}

int DCE_count=0;

void RunDeadCodeElimination(LLVMModuleRef Module) 
{

}


int main (int argc, char ** argv)
{
/* some basic error handling */
  if (argc < 3) {
    fprintf(stderr,"Not enough files specified.\n");
    return 0;
  }

  LLVMMemoryBufferRef Buff=NULL;
  char *outMessage=NULL;

  /* 1. Read contents of object file from command line argv[1] */  
  LLVMCreateMemoryBufferWithContentsOfFile(argv[1],&Buff,&outMessage);

  LLVMModuleRef Module=NULL;
  /* 2. Try to parse buffer into a legal Module */
  if (!Buff || LLVMParseBitcode(Buff,&Module,&outMessage))
    {
      /* handle error */
      printf("Error opening file: %s\n",outMessage);
      return 1;
    }
  
  /* 3. Do optimization on Module */

  
  /* 4. Save result to a new file (argv[2]) to preserve original */
  char *msg;
  LLVMBool res = LLVMVerifyModule(Module, LLVMPrintMessageAction, &msg);
  if (!res) 
    LLVMWriteBitcodeToFile(Module,argv[2]);
  else
    fprintf(stderr,"Error: %s not created.\n",argv[2]); 

  fprintf(stderr,"Removed %d instructions.",DCE_count);

  return 0;
}
