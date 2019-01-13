#include <assert.h>

#define chatting printf
#define __Olden_panic assert
#define ALLOC(nd,sz) (char *)malloc(sz)
#define MAX_PROC 1 

#include "hash.h"
 
typedef struct vert_st {
  int mindist;
  struct vert_st *next ;
  Hash edgehash;
  } *Vertex;

typedef struct graph_st {
  Vertex vlist[MAX_PROC];
  } *Graph;

Graph MakeGraph(int numvert);
