#ifndef MYTRANSFORM_H
#define MYTRANSFORM_H

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

LLVMValueRef InstructionSimplify(LLVMValueRef I);


#ifdef __cplusplus
}
#endif

#endif
