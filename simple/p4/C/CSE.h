#ifndef CSE_H
#define CSE_H

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

void LLVMCommonSubexpressionElimination(LLVMModuleRef Module);

#ifdef __cplusplus
}
#endif

#endif
