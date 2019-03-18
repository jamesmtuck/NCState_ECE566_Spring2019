#ifndef DOMINANCE_H
#define DOMINANCE_H

#include "llvm/Support/DataTypes.h"
#include "llvm-c/Core.h"

#ifdef __cplusplus

/* Need these includes to support the LLVM 'cast' template for the C++ 'wrap' 
   and 'unwrap' conversion functions. */
#include "llvm/IR/Module.h"
#include "llvm/PassRegistry.h"
#include "llvm/IR/IRBuilder.h"

extern "C" {
#endif

LLVMBool LLVMDominates(LLVMValueRef Fun, LLVMBasicBlockRef a, LLVMBasicBlockRef b);
LLVMBool LLVMPostDominates(LLVMValueRef Fun, LLVMBasicBlockRef a, LLVMBasicBlockRef b);

LLVMBasicBlockRef LLVMImmDom(LLVMBasicBlockRef BB);
LLVMBasicBlockRef LLVMImmPostDom(LLVMBasicBlockRef BB);

LLVMBasicBlockRef LLVMNearestCommonDominator(LLVMBasicBlockRef A, LLVMBasicBlockRef B);
unsigned LLVMGetLoopNestingDepth(LLVMBasicBlockRef BB);

LLVMBasicBlockRef LLVMFirstDomChild(LLVMBasicBlockRef BB);
LLVMBasicBlockRef LLVMNextDomChild(LLVMBasicBlockRef BB, LLVMBasicBlockRef Child);

  LLVMBool LLVMIsReachableFromEntry(LLVMValueRef Fun, LLVMBasicBlockRef bb);
#ifdef __cplusplus
}
#endif

#endif
