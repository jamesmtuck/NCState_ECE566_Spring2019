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
#include "llvm/Support/CBindingWrapping.h"
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

#include "loop.h"
#include "worklist.h"

using namespace llvm;

typedef LoopInfoBase<BasicBlock,Loop> LoopInfoBase2;

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(LoopInfoBase2,LLVMLoopInfoRef)
DEFINE_SIMPLE_CONVERSION_FUNCTIONS(Loop,LLVMLoopRef)

LLVMLoopInfoRef LLVMCreateLoopInfoRef(LLVMValueRef Fun) {
  LoopInfoBase<BasicBlock,Loop> *LI = new LoopInfoBase<BasicBlock,Loop>();
  DominatorTreeBase<BasicBlock,false> *DT = new DominatorTreeBase<BasicBlock,false>();
  DT->recalculate(*(Function*)unwrap(Fun));
  LI->analyze(*DT);
  return wrap(LI);
}

LLVMLoopRef LLVMGetLoopRef(LLVMLoopInfoRef LIRef,LLVMBasicBlockRef BBRef)
{
  LoopInfoBase<BasicBlock,Loop> *LI = unwrap(LIRef);
  BasicBlock *BB = unwrap(BBRef);
  return wrap(LI->getLoopFor(BB));
}

worklist_t LLVMGetBlocksInLoop(LLVMLoopRef LRef)
{
  Loop *L = unwrap(LRef);
  Loop::block_iterator bit;
  worklist_t wlist = worklist_create();
  for(bit=L->block_begin(); bit!=L->block_end(); bit++) {
    worklist_insert(wlist,LLVMBasicBlockAsValue(wrap(*bit)));
  }
  return wlist;
}

worklist_t LLVMGetExitBlocks(LLVMLoopRef LRef)
{
  Loop *L = unwrap(LRef);
  SmallVector<BasicBlock*,32> exits;
  L->getExitBlocks(exits);

  SmallVectorImpl<BasicBlock*>::iterator bit;
  worklist_t wlist = worklist_create();
  for(bit=exits.begin(); bit!=exits.end(); bit++) {
    worklist_insert(wlist,LLVMBasicBlockAsValue(wrap(*bit)));
  }
  return wlist;
}

LLVMBasicBlockRef LLVMGetPreheader(LLVMLoopRef LRef)
{
  Loop *L = unwrap(LRef);
  return wrap(L->getLoopPreheader());
}

/*LLVMBool LLVMHasDedicatedExits(LLVMLoopRef LRef)
{
  Loop *L = unwrap(LRef);
  return L->hasDedicatedExits();
  }*/

LLVMBasicBlockRef LLVMGetSingleExit(LLVMLoopRef LRef)
{
  Loop *L = unwrap(LRef);
  BasicBlock *bb = L->getExitBlock();
  return wrap(bb);
}

LLVMBool LLVMIsValueLoopInvariant(LLVMLoopRef LRef,LLVMValueRef Val)
{
  Loop *L = unwrap(LRef);
  return L->isLoopInvariant(unwrap(Val));
}

LLVMBool LLVMMakeLoopInvariant(LLVMLoopRef LRef,LLVMValueRef Val)
{
  Loop *L = unwrap(LRef);
  bool changed=false;
  L->makeLoopInvariant(unwrap(Val),changed);
  return changed;
}

LLVMLoopRef LLVMGetFirstLoop(LLVMLoopInfoRef LIRef) 
{
  LoopInfoBase2 * LI = unwrap(LIRef);
  LoopInfoBase2::iterator it=LI->begin();
  if(it==LI->end())
    return NULL;
  return wrap(*it);
}

LLVMLoopRef LLVMGetNextLoop(LLVMLoopInfoRef LIRef, LLVMLoopRef Loop) 
{
  LoopInfoBase2 * LI = unwrap(LIRef);
  LoopInfoBase2::iterator it=LI->begin();
  bool found=false;
  for(;it!=LI->end(); it++) {
    if (found)
      return wrap(*it);
    if(*it==unwrap(Loop))
      found=true;
  }
  return NULL;
}

LLVMBool LLVMLoopContainsInst(LLVMLoopRef L, LLVMValueRef Insn)
{
  Loop *l = unwrap(L);
  Instruction *I = (Instruction*)unwrap(Insn);
  return l->contains(I);
}

LLVMBool LLVMLoopContainsBasicBlock(LLVMLoopRef L, LLVMBasicBlockRef BB)
{
  Loop *l = unwrap(L);
  return l->contains(unwrap(BB));
}
