#ifndef WORKLIST_H
#define WORKLIST_H

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

typedef void * worklist_t;

/* Create an empty worklist */
worklist_t worklist_create();

void worklist_destroy(worklist_t);
worklist_t worklist_for_function(LLVMValueRef Function);
worklist_t worklist_for_basicblock(LLVMBasicBlockRef BasicBlock);

/* Insert a new value into worklist */
void worklist_insert(worklist_t w, LLVMValueRef val);

/* Check if empty */
LLVMBool worklist_empty(worklist_t w);

/* Get next data to pop */
LLVMValueRef worklist_top(worklist_t w);

/* Get and remove top from list */
LLVMValueRef worklist_pop(worklist_t w);

#ifdef __cplusplus
}
#endif

#endif
