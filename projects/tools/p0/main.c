#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"


int Summarize(LLVMModuleRef Module)
{
  
  
  
}

int
main (int argc, char ** argv)
{
  if (argc < 3) {
    fprintf(stderr,"Not enough positional arguments to %s.\n",argv[0]);
    return 1;
  }

  LLVMMemoryBufferRef Buff=NULL;
  char *outMessage=NULL;

  LLVMCreateMemoryBufferWithContentsOfFile(argv[1],&Buff,&outMessage);
  LLVMContextRef Context = LLVMContextCreate();

  LLVMModuleRef Module=NULL;
  if (!LLVMParseBitcode(Buff,&Module,&outMessage))
    {
      /* handle error */
    }


  Summarize(Module);


  LLVMWriteBitcodeToFile(Module,argv[2]);

  return 0;
}
