/* =================== PROGRAM bitonic===================== */
/* UP - 0, DOWN - 1 */
#include "node.h"   /* Node Definition */
#include "proc.h"   /* Procedure Types/Nums */

#define CONST_m1 10000
#define CONST_b 31415821
#define RANGE 100
#define NULL 0

#include <stdio.h>
#include <stdlib.h>
#include "mem-ref.h"
#include "future-cell.h"

#ifdef VERIFY_AFFINITIES
#include "affinity.h"
CHECK2(HANDLE *,left,right,tree)
#endif

#define put(a) chatting("%d",a)
#define puts(a) chatting(a)
#ifdef NOTESTP
#define NOTEST()
#define RETEST()
#define mymalloc malloc
#endif


int flag=0,foo=0;
int __NumNodes,__NDim;

#define LocalNewNode(h,v) \
  \
{ \
    h = (HANDLE *) mymalloc(sizeof(struct node)); \
    NOTEST(); \
      h->value = v; \
	h->left = NIL; \
	  h->right = NIL; \
    RETEST(); \
	  };

#ifndef ONEONLY
void NewSwapSubTree();
#define NewNode(h,v,procid) \
\
{ \
  h = (HANDLE *) makenode(procid); \
  NOTEST(); \
  h->value = v; \
  h->left = NIL; \
  h->right = NIL; \
  RETEST(); \
};
#else
#define NewNode(h,v,procid) LocalNewNode(h,v)
#endif

void 
/********/
InOrder(h)
/********/
HANDLE *h;
{
int val;
HANDLE *l, *r;
if ((h != NIL))
  {
   val = h->value;
   l = h->left;
   r = h->right;
   InOrder(l);
   chatting("%d @ 0x%x\n",val,(unsigned int)h);
   InOrder(r);
  }
}
int mult(int p, int q)
{
	int p1, p0, q1, q0;
	
	p1=p/CONST_m1; p0=p%CONST_m1;
	q1=q/CONST_m1; q0=q%CONST_m1;
	return (((p0*q1+p1*q0) % CONST_m1)*CONST_m1+p0*q0);
}

static int myrandom(int seed)
{
  int tmp;
  tmp = (mult(seed,CONST_b)+1);
  return tmp;
}

int skiprand(int seed, int n)
/* Generate the nth random # */
{
  for (; n; n--) seed=myrandom(seed);
  return seed;
}


HANDLE* RandTree(n,seed,node,level)
     int n,seed,node,level;

{
  int next_val,my_name;
  future_cell_int f_left, f_right;
  HANDLE *h;
  my_name=foo++;
  if ((n > 1))
    {
      int newnode;
      if (level<__NDim)
	{
	  newnode = node + (1 <<  (__NDim-level-1));
	}
      else
	newnode = node;
      seed = myrandom(seed);
      next_val=seed % RANGE;
      NewNode(h,next_val,node);
      f_left.value=(int) RandTree((n/2),seed,newnode,level+1);
      f_right.value=(int) RandTree((n/2),skiprand(seed,(n)+1),node,level+1);
      h->left = (HANDLE *) f_left.value;
      h->right = (HANDLE *) f_right.value;
    }
  else 
    h = NIL;
  return(h);
} 

void
/************/
SwapValue(l,r)
/************/
HANDLE *l;
HANDLE *r;

{ int temp,temp2;
  
  temp = l->value;
  temp2 = r->value;
  NOTEST();
  r->value = temp;
  RETEST();
  l->value = temp2;
} 

#ifndef ONEONLY
void SwapTree(t1, t2)
     HANDLE *t1, *t2;
{
  if ((t1==NULL) || (t2==NULL)) return;
  if (PID(t1) != PID(t2))
    {
      int t1val, t2val;
      HANDLE *t1left, *t1right, *t2left, *t2right;
  
      /*chatting("Swap 0x%x,0x%x\n",t1,t2);*/
      t1val = t1->value;
      NOTEST();
      t1left = t1->left;
      t1right = t1->right;
      RETEST();
      t2val = t2->value;
      NOTEST();
      t2left = t2->left;
      t2right = t2->right;
      t2->value = t1val;
      RETEST();
      t1->value = t2val;
      /*chatting("values were %d,%d\n",t1val,t2val);*/
      if (PID(t1) != PID(t1left))
	{
	  future_cell_int fcleft;
	  FUTURE(t1left,t2left,SwapTree,&fcleft);
     SwapTree(t1right,t2right);
	  TOUCH(&fcleft); 
/*	  SwapTree(t1left,t2left);
	  SwapTree(t1right,t2right);*/
	}
      else 
	{
	  future_cell_int fcleft;
	  FUTURE(t1left,t2left,NewSwapSubTree,&fcleft);
     NewSwapSubTree(t1right,t2right);
	  TOUCH(&fcleft);
/*	  NewSwapSubTree(t1left,t2left);
	  NewSwapSubTree(t1right,t2right); */
	}
    }
  else 
    {
      int tempval;
      HANDLE *temphandle;
      NOTEST();
      /*chatting("I think I'm local\n");*/
      tempval=t1->value;
      t1->value=t2->value;
      t2->value=tempval;
      temphandle=t1->left;
      t1->left=t2->left;
      t2->left=temphandle;
      temphandle=t1->right;
      t1->right=t2->right;
      t2->right=temphandle;
      RETEST();
    }
	
  /*chatting("End Swap 0x%x,0x%x\n",t1,t2);*/
}

#endif

void
/***********/
SwapValLeft(l,r,ll,rl,lval,rval)
/***********/
HANDLE *l;
HANDLE *r;
HANDLE *ll;
HANDLE *rl;
int lval, rval;
{
  r->value = lval;
#ifdef ONEONLY  
  r->left = ll;
  l->left = rl;
#else
  if (rl) SwapTree(rl,ll);
#endif  
  l->value = rval;
} 


void
/************/
SwapValRight(l,r,lr,rr,lval,rval)
/************/
HANDLE *l;
HANDLE *r;
HANDLE *lr;
HANDLE *rr;
int lval, rval;
{  
  r->value = lval;
#ifdef ONEONLY
  r->right = lr;
  l->right = rr;
#else
  if (lr) SwapTree(lr,rr);
#endif  
  l->value = rval;
  /*chatting("Swap Val Right l 0x%x,r 0x%x val: %d %d\n",l,r,lval,rval);*/
} 

int
/********************/
Bimerge(root,spr_val,dir)
/********************/
HANDLE *root;
int spr_val,dir;

{ int rightexchange;
  int elementexchange;
  int temp;
  HANDLE *pl,*pll,*plr;
  HANDLE *pr,*prl,*prr;
  HANDLE *rl;
  HANDLE *rr;
#ifdef  DUMB
  HANDLE *dummy;
#endif  
  int rv,lv;


  /*chatting("enter bimerge %x\n", root);*/
  rv = root->value;
  NOTEST();
  pl = root->left;
  pr = root->right;
  rightexchange = ((rv > spr_val) ^ dir);
  if (rightexchange)
    {
      root->value = spr_val;
      spr_val = rv;
    }
  RETEST();
  
  while ((pl != NIL))
    {
      /*chatting("pl = 0x%x,pr = 0x%x\n",pl,pr);*/
      lv = pl->value;
      NOTEST();
      pll = pl->left;
      plr = pl->right;
      RETEST();
      rv = pr->value;
      NOTEST();
      prl = pr->left;
      prr = pr->right;
      RETEST();
      elementexchange = ((lv > rv) ^ dir);
      if (rightexchange)
        if (elementexchange)
          { 
            SwapValRight(pl,pr,plr,prr,lv,rv);
            pl = pll;
            pr = prl;
          }
        else 
          { pl = plr;
            pr = prr;
          }
      else 
        if (elementexchange)
          { 
            SwapValLeft(pl,pr,pll,prl,lv,rv);
            pl = plr;
            pr = prr;
          }
        else 
          { pl = pll;
            pr = prl;
          }
    }
  if ((root->left != NIL))
    { 
      future_cell_int f_left;
      int value;
      NOTEST();
      rl = root->left;
      rr = root->right;
      value = root->value;
#ifdef DUMB
      LocalNewNode(dummy,value);
#endif      
      root->value=Bimerge(rl,value,dir);
      spr_val=Bimerge(rr,spr_val,dir);
      RETEST();
    }
  /*chatting("exit bimerge %x\n", root);*/
  return spr_val;
} 

int
/*******************/
Bisort(root,spr_val,dir)
/*******************/
HANDLE *root;
int spr_val,dir;

{ HANDLE *l;
  HANDLE *r;
#ifdef DUMB
  HANDLE *dummy;
#endif  
  int val;
  /*chatting("bisort %x\n", root);*/
  if (root->left == NIL)
    { 
     NOTEST();
     if (((root->value > spr_val) ^ dir))
        {
	  val = spr_val;
	  spr_val = root->value;
	  root->value =val;
	}
    RETEST();
    }
  else 
    {
      future_cell_int f_left;
      int ndir;
      NOTEST();
      l = root->left;
      r = root->right;
      val = root->value;
      /*chatting("root 0x%x, l 0x%x, r 0x%x\n", root,l,r);*/
#ifdef DUMB 
      LocalNewNode(dummy,val);
#endif      
      root->value=Bisort(l,val,dir);
      ndir = !dir;
      spr_val=Bisort(r,spr_val,ndir);
      spr_val=Bimerge(root,spr_val,dir);
      RETEST();
    }
  /*chatting("exit bisort %x\n", root);*/
  return spr_val;
} 

int
/*********/
main(argc,argv)
int argc;
char *argv[];
/*********/

{ HANDLE *h;
  unsigned long time1, time2, time3, time4;
  int sval;
  int height;
  int counter;
  int n;
  int result = 1;
   
  
  n = dealwithargs(argc,argv);

  chatting("Bisort with %d size on %d procs of dim %d\n",
	   n, __NumNodes, __NDim);
  h = RandTree(n,12345768,0,0);
  sval = myrandom(245867) % RANGE;
  if (flag) {
    InOrder(h);
    chatting("%d\n",sval);
   }
  chatting("**************************************\n");
  chatting("BEGINNING BITONIC SORT ALGORITHM HERE\n");
  chatting("**************************************\n");

#ifdef VERIFY_AFFINITIES
  Docheck_tree(h);
#endif


  sval=Bisort(h,sval,0);
  if (flag) {
    chatting("Sorted Tree:\n"); 
    InOrder(h);
    chatting("%d\n",sval);
   }
  
  sval=Bisort(h,sval,1);
  if (flag) {
    chatting("Sorted Tree:\n"); 
    InOrder(h);
    chatting("%d\n",sval);
   }

  return 0;
} 

