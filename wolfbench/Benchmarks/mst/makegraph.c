#include "mst.h"
#include <stdlib.h>

#define CONST_m1 10000
#define CONST_b 31415821
#define RANGE 2048
static int HashRange;

static int mult(int p, int q)
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

static int compute_dist(int i,int j, int numvert)
{
  int less, gt;
  if (i<j) {less = i; gt = j;} else {less = j; gt = i;}
  return (myrandom(less*numvert+gt) % RANGE)+1;
}

static int hashfunc(unsigned int key)
{
  return ((key>>10) % HashRange);
}

static void AddEdges(int count1, Graph retval, int numproc, 
                     int perproc, int numvert, int j) 
{
  Vertex tmp;
  Vertex helper[MAX_PROC];
  int i;

  for (i=0; i<numproc; i++) {
    helper[i] = retval->vlist[i];
  }

  for (tmp = retval->vlist[j]; tmp; tmp=tmp->next) 
    {
      for (i=0; i<numproc*perproc; i++) 
        {
          int pn,offset,dist;
          Vertex dest;
          Hash hash;
          
          if (i!=count1) 
            {
              dist = compute_dist(i,count1,numvert);
              pn = i/perproc;
              offset = i % perproc;
              dest = ((helper[pn])+offset);
              hash = tmp->edgehash;
              HashInsert((void *) dist,(unsigned int) dest,hash);
            }
        } /* for i... */
      count1++;
    } /* for tmp... */
}

Graph MakeGraph(int numvert) 
{
  int perproc = numvert;
  int i;
  int count1;
  Vertex v,tmp;
  Vertex block;
  Graph retval;
#ifdef FUTURES
  future_cell_int fc[MAX_PROC];
#endif

  retval = (Graph) ALLOC(0,sizeof(*retval));
  for (i=0; i<MAX_PROC; i++) 
    {
      retval->vlist[i]=NULL;
    }

  chatting("Make phase 2, numvert %d, numproc %d\n",numvert,1);
  {
    block = (Vertex) ALLOC(0,perproc*(sizeof(*tmp)));
    v = NULL;
    for (i=0; i<perproc; i++) 
      {
        tmp = block+(perproc-i-1);
        HashRange = numvert/4;
        tmp->mindist = 9999999;
        tmp->edgehash = MakeHash(numvert/4,hashfunc);
        tmp->next = v;
        v=tmp;
      }
    retval->vlist[0] = v;
  }

  chatting("Make phase 3\n");
  {
    count1 = 0*perproc;
    AddEdges(count1, retval, 1, perproc, numvert, 0);
  } 

  chatting("Make returning\n");
  return retval;
}

  
