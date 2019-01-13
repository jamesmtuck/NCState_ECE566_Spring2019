#include "mst.h"

typedef struct blue_return {
  Vertex vert;
  int dist;
} BlueReturn;

static BlueReturn BlueRule(Vertex inserted, Vertex vlist) 
{
  BlueReturn retval;
  Vertex tmp,prev;
  Hash hash;
  int dist,dist2;
  int count;
  
  if (!vlist) {
    retval.dist = 999999;
    return retval;
  }
  prev = vlist;
  retval.vert = vlist;
  retval.dist = vlist->mindist;
  hash = vlist->edgehash;
  dist = (int) HashLookup((unsigned int) inserted, hash);
  /*chatting("Found %d at 0x%x for 0x%x [0x%x]\n",dist,inserted,hash,vlist);*/
  if (dist) 
    {
      if (dist<retval.dist) 
        {
          vlist->mindist = dist;
          retval.dist = dist;
        }
    }
  else __Olden_panic("Not found\n");
  
  count = 0;
  /* We are guaranteed that inserted is not first in list */
  for (tmp=vlist->next; tmp; prev=tmp,tmp=tmp->next) 
    {
      count++;
      if (tmp==inserted) 
        {
          Vertex next;

          next = tmp->next;
          prev->next = next;
        }
      else 
        {
          hash = tmp->edgehash;
          dist2 = tmp->mindist;
          dist = (int) HashLookup((unsigned int) inserted, hash);
          /*chatting("Found %d at 0x%x for 0x%x *[0x%x]\n",dist,inserted,hash,tmp);*/
          if (dist) 
            {
              if (dist<dist2) 
                {
                  tmp->mindist = dist;
                  dist2 = dist;
                }
            }
          else __Olden_panic("Not found\n");
          if (dist2<retval.dist) 
            {
              retval.vert = tmp;
              retval.dist = dist2;
            }
        } /* else */
    } /* for */
  /*chatting("Count was %d\n",count);*/
  return retval;
}

          

static Vertex MyVertexList = NULL;

static void SetMyVertexList(Vertex v) {
  MyVertexList = v;
}

static BlueReturn Do_all_BlueRule(Vertex inserted, int pn) {
  BlueReturn retright;

  if (inserted == MyVertexList)
    MyVertexList = MyVertexList->next;
  return BlueRule(inserted,MyVertexList);
}

static int ComputeMst(Graph graph,int numvert) 
{
  Vertex v;
  Vertex inserted,tmp;
  int cost=0,i,dist;

  /* make copy of graph */
  chatting("Compute phase 1\n");

  /* Insert first node */
  inserted = graph->vlist[0];
  tmp = inserted->next;
  graph->vlist[0] = tmp;
  MyVertexList = tmp;
  numvert--;

  /* Announce insertion and find next one */
  chatting("Compute phase 2\n");
  while (numvert) 
    {
      BlueReturn br;
      
      br = Do_all_BlueRule(inserted,0);
      inserted = br.vert;    
      dist = br.dist;
      numvert--;
      cost = cost+dist;
    }
  return cost;
}

int main(int argc, char *argv[]) 
{
  Graph graph;
  int dist;
  int size;
 
  size = dealwithargs(argc,argv);
  chatting("Making graph of size %d\n",size);
  graph = MakeGraph(size);
  chatting("Graph completed\n");

  chatting("About to compute mst \n");
  dist = ComputeMst(graph,size);
  chatting("MST has cost %d\n",dist);

  return 0;
}
