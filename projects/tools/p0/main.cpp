#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/Bitcode/ReaderWriter.h"

#include "llvm/IR/IRPrintingPasses.h" 
#include "llvm/Bitcode/BitcodeWriterPass.h"
#include "llvm/IR/LegacyPassNameParser.h"
#include "llvm/IR/Verifier.h"
#include "llvm/InitializePasses.h"

#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/RegionPass.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Target/TargetLibraryInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/Triple.h"

#include "llvm/Support/Signals.h"
#include "llvm/Support/Debug.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/FileSystem.h"

#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <memory>
#include <algorithm>
using namespace llvm;


int Summarize(Module *M)
{
  
  
  return 0;
}

int
main (int argc, char ** argv)
{
  if (argc < 3) {
    fprintf(stderr,"Not enough positional arguments to %s.\n",argv[0]);
    return 1;
  }

  llvm_shutdown_obj Y;  // Call llvm_shutdown() on exit.
  LLVMContext &Context = getGlobalContext();

  std::string InputFilename(argv[1]);
  std::string OutputFilename(argv[2]);

  std::unique_ptr<tool_output_file> Out;
  
  std::string ErrorInfo;
  std::error_code EC;
  Out.reset(new tool_output_file(OutputFilename.c_str(), EC,
				 sys::fs::F_None));

  SMDiagnostic Err;

  std::unique_ptr<Module> M = parseIRFile(InputFilename, Err, Context);

  if (M.get() == 0) {
    Err.print(argv[0], errs());
    return 1;
  }

  Summarize(M.get());

  WriteBitcodeToFile(M.get(),Out->os());

  Out->keep();

  return 0;
}
