#ifndef VALMAP_H
#define VALMAP_H

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

typedef void * valmap_t;

  /* Create a valmap */
valmap_t valmap_create();

void valmap_destroy(valmap_t);

  /* Insert a new key-data pair into map */
void valmap_insert(valmap_t map, LLVMValueRef key, void*data);

  /* Check if key exists */
LLVMBool valmap_check(valmap_t map, LLVMValueRef key);

  /* Get data for matching key */
void *valmap_find(valmap_t map, LLVMValueRef key);

#ifdef __cplusplus
}
#endif

#endif
