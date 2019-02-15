#include "list.h"
#include "stdlib.h"

paramlist_t* push_param(paramlist_t *head, const char *name, Type* type)
{
  paramlist_t * node = (paramlist_t*) malloc(sizeof(paramlist_t));
  node->next = head;
  node->name = name;
  node->type = type;
  return node;
}

paramlist_t* pop_param(paramlist_t *head)
{
  paramlist_t *next = head->next;
  free(head);
  return next;
}

paramlist_t* next_param(paramlist_t *head)
{
  paramlist_t *next = head->next;
  return next;
}

int paramlist_size(paramlist_t *head)
{
  int cnt=0;
  while(head)
    {
      cnt++;
      head=head->next;
    }
  
  return cnt;
}

typedef struct loop_list {
  loop_info_t info;
  struct loop_list *next;
} loop_list_t;

static loop_list_t *head = NULL;

void push_loop(BasicBlock* expr,
	       BasicBlock* body,
	       BasicBlock* reinit,
	       BasicBlock* exit)
{
  loop_list_t *n = (loop_list_t*) malloc(sizeof(loop_list_t));
  n->info.expr = expr;
  n->info.body = body;
  n->info.reinit = reinit;
  n->info.exit = exit;

  n->next = head;
  head = n;
}

void pop_loop()
{
  loop_list_t *tmp = head;
  head = head->next;
  free(tmp);
}

loop_info_t get_loop()
{
  loop_info_t n;
  n.expr = n.body = n.reinit = n.exit = NULL;
  
  if (head)
    return head->info;
  else
    return n;
}
