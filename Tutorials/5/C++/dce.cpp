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
#include <set>

using namespace llvm;

static LLVMContext TheContext;

int DCE_count=0;

void NoOptimization(Module *M)
{
  printf("NoOptimization is running!\n");

  for(Module::iterator mit = M->begin(); 
      mit != M->end(); mit++)
    {
      //Function &F = *mit;
      //Function *F_ptr = &*mit;

      for(Function::iterator fit = F.begin(); 
	  fit != F.end();
	  fit++)
	{
	  BasicBlock &BB = *fit;

	  for(BasicBlock::iterator bit = BB.begin();
	      bit != BB.end();
	      bit++)
	    {
	      Instruction &I = *bit;
	      LLVMDumpValue(wrap(&I));
	    }


	}

    }
  
}


int isDead(Instruction * i)
{
  // All conditions passed
  LLVMValueRef I = wrap(i);
  
  // Not dead, because there are uses
  if(LLVMGetFirstUse(I)!=NULL)
    return 0;
  
  LLVMOpcode opcode = LLVMGetInstructionOpcode(I);
  switch(opcode) {
  // when in doubt, keep it! add opcode here to remove:
  case LLVMAdd:
  case LLVMFAdd: 	
  case LLVMSub:
  case LLVMFSub: 	
  case LLVMMul:
  case LLVMFMul: 	
  case LLVMUDiv:	
  case LLVMSDiv:	
  case LLVMFDiv:	
  case LLVMURem: 	
  case LLVMSRem: 	
  case LLVMFRem: 	
  case LLVMShl: 	
  case LLVMLShr: 	
  case LLVMAShr: 	
  case LLVMAnd: 	
  case LLVMOr: 	
  case LLVMXor: 	
  case LLVMAlloca:
  case LLVMGetElementPtr: 	
  case LLVMTrunc: 	
  case LLVMZExt: 	
  case LLVMSExt: 	
  case LLVMFPToUI: 	
  case LLVMFPToSI: 	
  case LLVMUIToFP: 	
  case LLVMSIToFP: 	
  case LLVMFPTrunc: 	
  case LLVMFPExt: 	
  case LLVMPtrToInt: 	
  case LLVMIntToPtr: 	
  case LLVMBitCast: 	
  case LLVMAddrSpaceCast: 	
  case LLVMICmp: 	
  case LLVMFCmp: 	
  case LLVMPHI: 
  case LLVMSelect: 
  case LLVMExtractElement: 	
  case LLVMInsertElement: 	
  case LLVMShuffleVector: 	
  case LLVMExtractValue: 	
  case LLVMInsertValue: 
    // Success!
    return 1;

  case LLVMLoad: if(!LLVMGetVolatile(I)) return 1; // Success

  // all others must be kept
  default:
    break;
  }

  // Failed
  return 0;
}


void RunDeadCodeElimination(Module *M)
{
  std::set<Instruction*> worklist;

  for(Module::iterator mit = M->begin(); 
      mit != M->end(); mit++)
    {
      Function &F = *mit;
      Function *F_ptr = &*mit;

      for(Function::iterator fit = F.begin(); 
	  fit != F.end();
	  fit++)
	{
	  BasicBlock &BB = *fit;

	  for(BasicBlock::iterator bit = BB.begin();
	      bit != BB.end();
	      bit++)
	    {
	      Instruction &I = *bit;
	      worklist.insert(&I);
	    }
	}

      while(worklist.size()>0) {
	Instruction* i = *worklist.begin();
	worklist.erase(i);
	
	if(isDead(i))
	  {
	    for(unsigned op=0; op<i->getNumOperands(); op++)
	      {
		if (  isa<Instruction>(i->getOperand(op)) ) {
		  Instruction *o = dyn_cast<Instruction>(i->getOperand(op));
		  worklist.insert(o);
		}
	      }
	    i->eraseFromParent();
	    DCE_count++;
	  }
      }     
    }
}



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

  RunDeadCodeElimination(M.get());
  //NoOptimization(M.get());

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
