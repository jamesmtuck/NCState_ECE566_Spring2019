#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FileSystem.h"
#include <memory>
#include <algorithm>

#include "llvm/Support/SourceMgr.h"
#include <memory>
#include <algorithm>
using namespace llvm;

#include "Summary.hpp"

int
main (int argc, char ** argv)
{
  char filename[1024];
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

  sprintf(filename,"%s.stats",argv[1]);
  Summarize(M.get(),"noopt", filename);

  WriteBitcodeToFile(*M.get(),Out->os());
  Out->keep();

  return 0;
}
