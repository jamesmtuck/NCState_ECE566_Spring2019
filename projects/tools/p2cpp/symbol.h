#ifndef SYMBOL_H
#define SYMBOL_H

#include "llvm/IR/Value.h"

using namespace llvm;

void symbol_push_scope();
void symbol_pop_scope();

int is_global_scope();

void symbol_insert(const char* name, Value* val);
Value* symbol_find(const char*);

#endif
