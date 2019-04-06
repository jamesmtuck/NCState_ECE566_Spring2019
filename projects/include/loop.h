#ifndef LOOP_H
#define LOOP_H

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

#include "worklist.h"


  typedef struct LLVMOpaqueLoopInfoRef* LLVMLoopInfoRef;
  typedef struct LLVMOpaqueLoopRef* LLVMLoopRef;

  LLVMLoopInfoRef LLVMCreateLoopInfoRef(LLVMValueRef Function);
  LLVMLoopRef LLVMGetLoopRef(LLVMLoopInfoRef,LLVMBasicBlockRef);
  worklist_t LLVMGetBlocksInLoop(LLVMLoopRef);
  worklist_t LLVMGetExitBlocks(LLVMLoopRef);
  LLVMBasicBlockRef LLVMGetSingleExit(LLVMLoopRef);

  LLVMLoopRef LLVMGetFirstLoop(LLVMLoopInfoRef LIRef);
  LLVMLoopRef LLVMGetNextLoop(LLVMLoopInfoRef LIRef, LLVMLoopRef Loop);

  LLVMBasicBlockRef LLVMGetPreheader(LLVMLoopRef);
  LLVMBasicBlockRef LLVMGetDedicatedExit(LLVMLoopRef);

  //DO NOT USE: LLVMBool LLVMHasDedicatedExits(LLVMLoopRef);  
  LLVMBool LLVMIsValueLoopInvariant(LLVMLoopRef,LLVMValueRef);
  LLVMBool LLVMMakeLoopInvariant(LLVMLoopRef,LLVMValueRef);
  
  LLVMBool LLVMLoopContainsInst(LLVMLoopRef, LLVMValueRef);
  LLVMBool LLVMLoopContainsBasicBlock(LLVMLoopRef, LLVMBasicBlockRef);
#ifdef __cplusplus



}
#endif

#endif
