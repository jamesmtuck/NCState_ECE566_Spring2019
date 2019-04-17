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
//#include "llvm/PassManager.h"
#include "llvm/IR/Dominators.h"
//#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Type.h"

#include "dominance.h"

using namespace llvm;

Function *Current=NULL;
DominatorTreeBase<BasicBlock,false> *DT=NULL;
DominatorTreeBase<BasicBlock,true> *PDT=NULL;

LoopInfoBase<BasicBlock,Loop> *LI=NULL;

void UpdateDominators(Function *F)
{
  if (Current != F)
    {
      Current = F;

      if (DT==NULL)
	{
	  DT = new DominatorTreeBase<BasicBlock,false>();
	  PDT = new DominatorTreeBase<BasicBlock,true>();
	  if (LI==NULL)
	    LI = new LoopInfoBase<BasicBlock,Loop>();
	}
      
      DT->recalculate(*F);
      PDT->recalculate(*F);

      LI->analyze(*DT);
    }
}

// Test if a dom b
LLVMBool LLVMDominates(LLVMValueRef Fun, LLVMBasicBlockRef a, LLVMBasicBlockRef b)
{
  UpdateDominators((Function*)unwrap(Fun));
  return DT->dominates(unwrap(a),unwrap(b));
}

// Test if a pdom b
LLVMBool LLVMPostDominates(LLVMValueRef Fun, LLVMBasicBlockRef a, LLVMBasicBlockRef b)
{
  UpdateDominators((Function*)unwrap(Fun));
  return PDT->dominates(unwrap(a),unwrap(b));
}

LLVMBool LLVMIsReachableFromEntry(LLVMValueRef Fun, LLVMBasicBlockRef bb) {
  UpdateDominators((Function*)unwrap(Fun));
  return DT->isReachableFromEntry(unwrap(bb));
}


LLVMBasicBlockRef LLVMImmDom(LLVMBasicBlockRef BB)
{
  UpdateDominators(unwrap(BB)->getParent());

  if ( DT->getNode((BasicBlock*)unwrap(BB)) == NULL )
    return NULL;
  
  if ( DT->getNode((BasicBlock*)unwrap(BB))->getIDom()==NULL )
    return NULL;

  return wrap(DT->getNode(unwrap(BB))->getIDom()->getBlock());
}

LLVMBasicBlockRef LLVMImmPostDom(LLVMBasicBlockRef BB)
{
  UpdateDominators(unwrap(BB)->getParent());

  if (PDT->getNode(unwrap(BB))->getIDom()==NULL)
    return NULL;

  return wrap((BasicBlock*)PDT->getNode(unwrap(BB))->getIDom()->getBlock());
}

LLVMBasicBlockRef LLVMFirstDomChild(LLVMBasicBlockRef BB)
{
  UpdateDominators(unwrap(BB)->getParent());
  DomTreeNodeBase<BasicBlock> *Node = DT->getNode(unwrap(BB));

  if(Node==NULL)
    return NULL;

  DomTreeNodeBase<BasicBlock>::iterator it = Node->begin();
  if (it!=Node->end())
    return wrap((*it)->getBlock());
  return NULL;
}

LLVMBasicBlockRef LLVMNextDomChild(LLVMBasicBlockRef BB, LLVMBasicBlockRef Child)
{
  UpdateDominators(unwrap(BB)->getParent());
  DomTreeNodeBase<BasicBlock> *Node = DT->getNode(unwrap(BB));
  DomTreeNodeBase<BasicBlock>::iterator it,end;

  bool next=false;
  for(it=Node->begin(),end=Node->end(); it!=end; it++)
    if (next)
      return wrap((*it)->getBlock());
    else if (*it==DT->getNode(unwrap(Child)))
      next=true;

  return NULL;
}


LLVMBasicBlockRef LLVMNearestCommonDominator(LLVMBasicBlockRef A, LLVMBasicBlockRef B)
{
  UpdateDominators(unwrap(A)->getParent());
  return wrap(DT->findNearestCommonDominator(unwrap(A),unwrap(B)));
}

unsigned LLVMGetLoopNestingDepth(LLVMBasicBlockRef BB)
{
  if (LI==NULL)
    UpdateDominators(unwrap(BB)->getParent());

  return LI->getLoopDepth(unwrap(BB));
}


LLVMBasicBlockRef LLVMDominanceFrontierLocal(LLVMBasicBlockRef BB)
{
  return NULL;
}

LLVMBasicBlockRef LLVMDominanceFrontierClosure(LLVMBasicBlockRef BB)
{
  return NULL;
}

LLVMBasicBlockRef LLVMPostDominanceFrontierLocal(LLVMBasicBlockRef BB)
{
  return NULL;
}

LLVMBasicBlockRef LLVMPostDominanceFrontierClosure(LLVMBasicBlockRef BB)
{
  return NULL;
}
