#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

#include "summary.h"

int
main (int argc, char ** argv)
{
  char filename[1024];
  if (argc < 3) {
    fprintf(stderr,"Not enough positional arguments to %s.\n",argv[0]);
    return 1;
  }

  LLVMMemoryBufferRef Buff=NULL;
  char *outMessage=NULL;

  LLVMCreateMemoryBufferWithContentsOfFile(argv[1],&Buff,&outMessage);
  //LLVMContextRef Context = LLVMContextCreate();

  LLVMModuleRef Module=NULL;
  if (!Buff || LLVMParseBitcode(Buff,&Module,&outMessage))
    {
      /* handle error */
      printf("Error handling file.\n");
      return 1;
    }
  
  sprintf(filename,"%s.stats",argv[1]);

  Summarize(Module,"noopt",(const char*)filename);

  LLVMWriteBitcodeToFile(Module,argv[2]);

  return 0;
}
