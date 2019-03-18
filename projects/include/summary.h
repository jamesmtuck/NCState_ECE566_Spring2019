#ifndef SUMMARY_H
#define SUMMARY_H

#include "llvm/Support/DataTypes.h"
#include "llvm-c/Core.h"

#ifdef __cplusplus

/* Need these includes to support the LLVM 'cast' template for the C++ 'wrap' 
   and 'unwrap' conversion functions. */
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"

void Summarize_Cpp(llvm::Module *M, const char *id, const char *filename);

extern "C" {
#endif

#include "llvm-c/Core.h"

void Summarize(LLVMModuleRef Module, const char *id, const char *filename);

#ifdef __cplusplus
}
#endif


#endif /* SUMMARY_H */
