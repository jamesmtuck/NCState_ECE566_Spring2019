#ifndef CFG_H
#define CFG_H

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

LLVMBool LLVMSinglePredecessor(LLVMBasicBlockRef BB);
LLVMBool LLVMSingleSuccessor(LLVMBasicBlockRef BB);

LLVMBasicBlockRef LLVMGetFirstSuccessor(LLVMBasicBlockRef BB);
LLVMBasicBlockRef LLVMGetNextSuccessor(LLVMBasicBlockRef BB, LLVMBasicBlockRef Succ);

LLVMBasicBlockRef LLVMGetFirstPredecessor(LLVMBasicBlockRef BB);
LLVMBasicBlockRef LLVMGetNextPredecessor(LLVMBasicBlockRef BB, LLVMBasicBlockRef Pred);

unsigned LLVMCountPredecessors(LLVMBasicBlockRef BB);

LLVMValueRef LLVMCloneInstruction(LLVMValueRef Insn);
LLVMValueRef LLVMFirstInstructionAfterPHI(LLVMBasicBlockRef);

#ifdef __cplusplus
}
#endif

#endif
