#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

extern FILE *yyin;
extern char *outfile;
extern int num_errors;

LLVMModuleRef Module;
LLVMContextRef Context;

extern char **targv;
extern char **arglim;
extern char *infile[];

extern void Cmdlex();
extern void usage();
extern int yywrap();
extern void yyparse();

int
main (int argc, char ** argv)
{
  arglim = argv+argc;
  targv = argv+1;

  Cmdlex();

  if (outfile==NULL)
    outfile = strdup("out.bc");

  if (!infile[0])
    {
      fprintf(stderr,"Incomplete command line. Usage:\n");
      usage();
      return 1;
    }

  /* Build global Context */
  Context = LLVMGetGlobalContext();

  /* Build global Module, this will be what we output */
  Module = LLVMModuleCreateWithNameInContext("main",Context);

  /* Set up initial value for yyin */
  yywrap();

  /* Parse MiniC input file */
  yyparse();

  /* If we encounter no errors, dump the Module */
  if (Module && num_errors==0)
    {
      LLVMWriteBitcodeToFile(Module,outfile);
    }
  else
    {
      fprintf(stderr,"Too many errors to print Module.\n");
    }

  /* Return an error status if it failed */
  return (num_errors==0)?EXIT_SUCCESS:EXIT_FAILURE;
}
