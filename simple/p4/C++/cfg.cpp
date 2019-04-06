/*
 * File: dominance.cpp
 *
 * Description:
 *   This provides a C interface to the dominance analysis in LLVM
 */

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
#include "llvm/IR/Dominators.h"
//#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Type.h"

#include "cfg.h"

using namespace llvm;

LLVMBool LLVMSinglePredecessor(LLVMBasicBlockRef BB)
{
  pred_iterator PI = pred_begin(unwrap(BB));
  if(PI!=pred_end(unwrap(BB)) && ++PI == pred_end(unwrap(BB)))
    return (LLVMBool)1;
  return (LLVMBool)0;
}

LLVMBool LLVMSingleSuccessor(LLVMBasicBlockRef BB)
{
  succ_iterator SI = succ_begin(unwrap(BB));
  if(SI!=succ_end(unwrap(BB)) && ++SI == succ_end(unwrap(BB)))
    return (LLVMBool)1;

  return (LLVMBool)0;
}

LLVMBasicBlockRef LLVMGetFirstSuccessor(LLVMBasicBlockRef BB)
{
  for (succ_iterator SI = succ_begin(unwrap(BB)), SE = succ_end(unwrap(BB));
            SI != SE; ++SI) {
    return wrap(*SI);
  }
  return NULL;
}

LLVMBasicBlockRef LLVMGetNextSuccessor(LLVMBasicBlockRef BB, LLVMBasicBlockRef Succ)
{
  bool next = false;
  for (succ_iterator SI = succ_begin(unwrap(BB)), SE = succ_end(unwrap(BB));
            SI != SE; ++SI) {
    if (next)
      return wrap(*SI);
    if (*SI==unwrap(Succ))
      next = true;
  }
  return NULL;
}

LLVMBasicBlockRef LLVMGetFirstPredecessor(LLVMBasicBlockRef BB)
{
  for (pred_iterator PI = pred_begin(unwrap(BB)), PE = pred_end(unwrap(BB));
            PI != PE; ++PI) {
    return wrap(*PI);
  }
  return NULL;
}

LLVMBasicBlockRef LLVMGetNextPredecessor(LLVMBasicBlockRef BB, LLVMBasicBlockRef Pred)
{
  bool next = false;
  for (pred_iterator PI = pred_begin(unwrap(BB)), PE = pred_end(unwrap(BB));
            PI != PE; ++PI) {
         // Does Node immediately dominate this successor?
    if (next)
      return wrap(*PI);
    if (*PI==unwrap(Pred))
      next = true;
  }
  return NULL;
}

unsigned LLVMCountPredecessors(LLVMBasicBlockRef BB)
{
  unsigned count=0;
  for (pred_iterator PI = pred_begin(unwrap(BB)), PE = pred_end(unwrap(BB));
       PI != PE; ++PI,++count);
  
  return count;
}
  
LLVMValueRef LLVMCloneInstruction(LLVMValueRef Insn)
{
  Instruction *insn = (Instruction*)unwrap(Insn);
  Instruction *clone = insn->clone();
  return wrap(clone);
}

LLVMValueRef LLVMFirstInstructionAfterPHI(LLVMBasicBlockRef BB)
{
  LLVMValueRef insn = LLVMGetFirstInstruction(BB);
  while( LLVMIsAPHINode(insn) )
    insn = LLVMGetNextInstruction(insn);
  return insn;
}
