#define COLLECT_SIZE 256
#define DFS_SIZE 20
#define NULL (HANDLE *) 0
#include "node.h"
#include "mem-ref.h"

typedef struct {
  int top;
  HANDLE *handles[DFS_SIZE];
} stack;

#define push(s,v) (s)->handles[++((s)->top)] = v
#define pop(s) (s)->handles[((s)->top)--]
#define stackempty(s) ((s)->top < 0)  
void VisitCollect(HANDLE *t, int *collect)
{
  register int val;
  val = t->value;
  *collect = val;
}

void VisitCollectReplace(HANDLE *t, int *collect)
{
  register int temp = *collect;
  register int val = t->value;
  *collect = val;
  t->value = temp;
}

void VisitReplace(HANDLE *t, int *collect)
{
  register int val = *collect;
  t->value = val;
}

void swapDFS(stack *s, int collection[], void visit())
{
  int num=0;
  
  while (!stackempty(s) && num < COLLECT_SIZE) 
    {
      HANDLE *v = pop(s);
      visit(v,&collection[num]);
      num++;
      if (v->left != NULL) 
	{
     HANDLE *child;
     child = v->right;
	  push(s,child);
     child = v->left;
	  push(s,child);
	}
    }
}

void NewSwapSubTree(HANDLE *t1, HANDLE *t2)
{
  stack c1, r1, s2;
  int collection[COLLECT_SIZE];

  /*chatting("starting swapping\n");*/

  if (t1!=NULL) 
    {
      c1.top = -1;
      r1.top = -1;
      s2.top = -1;
      push(&c1,t1);
      push(&r1,t1);
      push(&s2,t2);
      while (!stackempty(&c1)) 
	{
	  swapDFS(&c1,collection,VisitCollect);
	  swapDFS(&s2,collection,VisitCollectReplace);
	  swapDFS(&r1,collection,VisitReplace);
	}
    }
  /*chatting("ending swapping\n");*/

}
  
  
int *Collect(HANDLE *t1, int collection[])
{
  register int val;
  HANDLE *tmp;
  if (t1==NULL) return collection;
  val = t1->value;
  *collection = val;
  collection += 1;
  tmp = t1->left;
  collection = Collect(tmp,collection);
  tmp = t1->right;
  collection = Collect(tmp,collection);
  return collection;
}

int *Collect_Replace(HANDLE *t1, int collection[])
{
  HANDLE *tmp;
  register int temp,val;
  if (t1==NULL) return collection;
  temp = *collection;
  val = t1->value;
  *collection = val;
  t1->value = temp;
  collection += 1;
  tmp = t1->left;
  collection = Collect_Replace(tmp,collection);
  tmp = t1->right;
  collection = Collect_Replace(tmp,collection);
  return collection;
}

int *Replace(HANDLE *t1, int collection[])
{
  HANDLE * tmp;
  register int val;
  if (t1==NULL) return collection;
  val = *collection;
  t1->value = val;
  collection +=1;
  tmp = t1->left;
  collection = Replace(tmp,collection);
  tmp = t1->right;
  collection = Replace(tmp,collection);
  return collection;
}


void SwapSubTree(HANDLE *t1, HANDLE *t2)
{
  int collection[COLLECT_SIZE];
  HANDLE *t1loc, *t2loc;

  Collect(t1,collection);
  Collect_Replace(t2,collection);
  Replace(t1,collection);
}
