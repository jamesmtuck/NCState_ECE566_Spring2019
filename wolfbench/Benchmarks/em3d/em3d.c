#include "em3d.h"

#ifdef VERIFY_AFFINITIES
#include "affinity.h"
CHECK1_ACCUM(node_t *,next,list)
#endif

#ifdef PLAIN
#define NOTEST()
#define RETEST()
#define LOCAL
#define ISLOCPTR
#endif
int nonlocals=0;
void compute_nodes(nodelist)
register node_t *nodelist;
{
  register int i;
  register node_t *localnode;
  
#ifdef VERIFY_AFFINITIES
  Accumulate_list(nodelist,nodelist);
#endif
  NOTEST();
  for (; nodelist; )
    {
      register double cur_value;
      register int from_count ;
	   register double *other_value;
	   register double coeff;
	   register double value;
      /*register double *coeffs;*/
      /*register node_t **from_nodes;*/
      
      localnode = LOCAL(nodelist);
      cur_value=*(localnode->value);
      from_count = localnode->from_count-1;
      for (i=0; i < from_count; i+=2)
	{
	  
     other_value = localnode->from_values[i];
     coeff = localnode->coeffs[i];
	  if (!ISLOCPTR(other_value)) nonlocals++;
	  RETEST();
	  value = *other_value;
	  NOTEST();
	  cur_value -= coeff*value;
     other_value = localnode->from_values[i+1];
     coeff = localnode->coeffs[i];
	  if (!ISLOCPTR(other_value)) nonlocals++;
	  RETEST();
	  value = *other_value;
	  NOTEST();
	  cur_value -= coeff*value;
	  
	  /*chatting("from %d, coeff %f, value %f\n",count,coeff,value);*/
	}
      if (i==from_count)  {
	other_value = localnode->from_values[i];
	coeff = localnode->coeffs[i];
	if (!ISLOCPTR(other_value)) nonlocals++;
	RETEST();
	value = *other_value;
	NOTEST();
	cur_value -= coeff*value;
      }
      *(localnode->value) = cur_value;
      nodelist = localnode->next;
    } /* for */
  RETEST();
}
