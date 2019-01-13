/* LLVM Header Files */
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Type.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/ADT/GraphTraits.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/Support/GraphWriter.h"
#include "llvm/IR/InstIterator.h"

#include <map>

#include "valmap.h"

using namespace llvm;

valmap_t valmap_create()
{
  ValueMap<Value*,void*> *map = new ValueMap<Value*,void*>();
  return (valmap_t)map;
}

void valmap_destroy(valmap_t map)
{
  ValueMap<Value*,void*> *vmap = (ValueMap<Value*,void*>*)map;
  delete vmap;
}

void valmap_insert(valmap_t map, LLVMValueRef v, void* data)
{
  ValueMap<Value*,void*> *vmap = (ValueMap<Value*,void*>*)map;
  (*vmap)[unwrap(v)] = data;
}

LLVMBool valmap_check(valmap_t map, LLVMValueRef v)
{
  ValueMap<Value*,void*> *vmap = (ValueMap<Value*,void*>*)map;
  Value *val = unwrap(v);
  return (LLVMBool)(vmap->find(val)!=vmap->end());
}

void *valmap_find(valmap_t map, LLVMValueRef v)
{
  ValueMap<Value*,void*> *vmap = (ValueMap<Value*,void*>*)map;
  Value *val = unwrap(v);
  
  if(vmap->find(val)==vmap->end())
    return NULL;
 
  return (*vmap)[val];
}
