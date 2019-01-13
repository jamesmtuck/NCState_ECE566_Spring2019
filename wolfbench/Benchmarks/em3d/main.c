#include "em3d.h"
#include "make_graph.h"

int DebugFlag;
int __NumNodes;

/***
void print_graph(graph_t *graph, int id) 
{
  node_t *cur_node;
  cur_node=graph->e_nodes[id];

  for(; cur_node; cur_node=cur_node->next)
    {
      chatting("E: value %f, from_count %d\n",cur_node->value,
	       cur_node->from_count);
    }
  cur_node=graph->h_nodes[id];
  for(; cur_node; cur_node=cur_node->next)
    {
      chatting("H: value %f, from_count %d\n",cur_node->value,
	       cur_node->from_count);
    }
}
***/

extern int nonlocals;

#ifndef PLAIN
void do_all_compute(graph_t *graph, int myid, int nproc)
{
  future_cell_int fleft;

  if (NONLOCAL(myid)) MIGRATE(myid);
  if (nproc>1) {
    FUTURE(graph,myid+nproc/2,nproc/2,do_all_compute,&fleft);
    do_all_compute(graph,myid,nproc/2);
    TOUCH(&fleft);
  }
  else
  {
    node_t *nodelist;

    nodelist=graph->e_nodes[myid];
    compute_nodes(nodelist);
    nodelist=graph->h_nodes[myid];
    compute_nodes(nodelist);
  }
}
#endif
int main(int argc, char *argv[])
{
  int i;
  graph_t *graph;

#ifndef PLAIN
  SPMDInit(argc,argv);
#else
  dealwithargs(argc,argv);
#endif
  chatting("Hello world--Doing em3d with args %d %d %d %d\n",
    n_nodes,d_nodes,local_p,__NumNodes);
  graph=initialize_graph();
  if (DebugFlag) 
    for(i=0; i<__NumNodes;i++)
      { //MIGRATE(i);
        /*print_graph(graph,i);*/
      }

#ifndef PLAIN
  MIGRATE(0);
#endif


/*   CMMD_node_timer_clear(0); */
/*   CMMD_node_timer_start(0); */
#ifndef PLAIN
  do_all_compute(graph,0,__NumNodes);
#else
  compute_nodes(graph->e_nodes[0]);
  compute_nodes(graph->h_nodes[0]);
#endif
/*   CMMD_node_timer_stop(0); */

  chatting("nonlocals = %d\n",nonlocals);
/*   chatting("Completed a computation phase %f\n",CMMD_node_timer_elapsed(0)); */
  printstats();
/*****
  for(i=0; i<__NumNodes;i++)
  { MIGRATE(i);
  print_graph(graph,i);
  }
*****/

#ifdef VERIFY_AFFINITIES
  Print_Accumulated_list();
#endif
/*   MIGRATE(0); */
#ifndef PLAIN
  __ShutDown(0);
#endif
  
  return 0;
}
