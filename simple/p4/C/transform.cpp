// add functions that Simplify Instructions and CFG

#include <stdio.h>
#include <stdlib.h>

/* LLVM Header Files */
#include "llvm-c/Core.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/GlobalVariable.h"
//#include "llvm/PassManager.h"
#include "llvm/IR/Dominators.h"
//#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Type.h"

#include "llvm/Analysis/InstructionSimplify.h"

#include "dominance.h"
#include "transform.h"

using namespace llvm;


LLVMValueRef InstructionSimplify(LLVMValueRef I)
{
  Instruction *II = (Instruction*)unwrap(I);
  const Module* M = II->getModule();
  return wrap( SimplifyInstruction((Instruction*)unwrap(I), M->getDataLayout() ));
}
