#include <stdlib.h>
#include <stdio.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IRReader/IRReader.h"

#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FileSystem.h"

#include "llvm/Support/SourceMgr.h"

#include "llvm-c/Core.h"
#include "llvm-c/Analysis.h"

#include <memory>
#include <algorithm>

using namespace llvm;

static LLVMContext TheContext;

void NoOptimization(Module *M)
{
  
  
}


int isDead(Instruction * i)
{
  // Failed
  return 0;
}


void RunDeadCodeElimination(Module *M)
{


}

int DCE_count=0;

int main (int argc, char ** argv)
{  
  if (argc < 3) {
    fprintf(stderr,"Not enough positional arguments to %s.\n",argv[0]);
    return 1;
  }

  std::string InputFilename(argv[1]);
  std::string OutputFilename(argv[2]);

  // LLVM idiom for constructing output file.
  std::unique_ptr<ToolOutputFile> Out;  
  std::string ErrorInfo;
  std::error_code EC;
  Out.reset(new ToolOutputFile(argv[2], EC,
  				 sys::fs::F_None));

  SMDiagnostic Err;
  std::unique_ptr<Module> M;
  LLVMContext *Context = new LLVMContext();
  M = parseIRFile(InputFilename, Err, *Context);

  if (M.get() == 0) {
    Err.print(argv[0], errs());
    return 1;
  }

  NoOptimization(M.get());

  M->print(errs(),nullptr);

  char *msg;
  LLVMBool res = LLVMVerifyModule(wrap(M.get()), LLVMPrintMessageAction, &msg);
  if (!res) {
    WriteBitcodeToFile(*M.get(),Out->os());
    Out->keep();
  } else {
    fprintf(stderr,"Error: %s not created.\n",argv[2]); 
  }

  fprintf(stderr,"Removed %d instructions.",DCE_count);

  return 0;
}
