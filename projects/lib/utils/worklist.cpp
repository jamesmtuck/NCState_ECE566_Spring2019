#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/BasicBlock.h"
#include <set>
#include "llvm/IR/InstIterator.h"
#include "worklist.h"

using namespace llvm;

typedef std::set<Value*> worklist_internal;
/* Create an empty worklist */
worklist_t worklist_create()
{
  worklist_internal *list = new std::set<Value*>();
  return (worklist_t) list;
}

void worklist_destroy(worklist_t w)
{
  worklist_internal *list = new worklist_internal();
  delete list;
}

worklist_t worklist_for_function(LLVMValueRef F)
{
  Function *Fun = unwrap<Function>(F);
  worklist_internal *list = new worklist_internal();
  
  for (inst_iterator I = inst_begin(Fun), E = inst_end(Fun); I != E; ++I)
    list->insert(&*I);
  
  return (worklist_t) list;
}

worklist_t worklist_for_basicblock(LLVMBasicBlockRef BBRef)
{
  BasicBlock *BB = unwrap(BBRef);
  BasicBlock::iterator I,E;
  worklist_internal *list = new worklist_internal();
  for(I=BB->begin(),E=BB->end(); I!=E; I++)
    {
      list->insert(&*I);
    }
  return (worklist_t) list;
}

/* Insert a new value into worklist */
void worklist_insert(worklist_t w, LLVMValueRef val)
{
  worklist_internal *list = (worklist_internal*)w;
  list->insert(unwrap(val));
}

/* Check if empty */
LLVMBool worklist_empty(worklist_t w)
{
  worklist_internal *list = (worklist_internal*)w;
  return (LLVMBool)list->empty();
}

/* Get next data to pop */
LLVMValueRef worklist_top(worklist_t w)
{
  worklist_internal *list = (worklist_internal*)w;
  worklist_internal::iterator i = list->begin();
  if(i!=list->end())
    return wrap(*i);
  else
    return NULL;
}

/* Get and remove top from list */
LLVMValueRef worklist_pop(worklist_t w)
{
  worklist_internal *list = (worklist_internal*)w;
  worklist_internal::iterator i =list->begin();
  if(i!=list->end())
    {
      LLVMValueRef ret = wrap(*i);
      list->erase(i);
      return ret;
    }
  else
    return NULL;
}
