#ifndef LIST_H
#define LIST_H

#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"

using namespace llvm;

typedef struct paramlist_def {
  struct paramlist_def *next;
  const char * name;
  Type*  type;
} paramlist_t;

paramlist_t* push_param(paramlist_t *head, const char *name, Type* type);
paramlist_t* pop_param(paramlist_t *head);
paramlist_t* next_param(paramlist_t *head);
int paramlist_size(paramlist_t *head);

void push_loop(BasicBlock* expr,
	       BasicBlock* body,
	       BasicBlock* reinit,
	       BasicBlock* exit);

void pop_loop();

typedef struct loop_info {
  BasicBlock* expr;
  BasicBlock* body;
  BasicBlock* reinit;
  BasicBlock* exit;
} loop_info_t;

loop_info_t get_loop();


#endif /*LIST_H*/
