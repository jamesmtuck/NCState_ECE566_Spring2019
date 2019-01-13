#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
extern double drand48();
extern long lrand48();

static int percentcheck=0,numlocal=0;

/* initialize the random number generator for a particular processor */
void init_random(int seed)
{
  srand48(seed);
}

/* return a random number from 0 to range-1 */
int gen_number(int range)
{
  return lrand48() % range;
}

/* return a random number in [-range,range] but not zero */
int gen_signed_number(int range)
{
  int temp;
 
  temp = lrand48() % (2*range);  /* 0..2*range-1 */
  temp =  temp-(range);
  if (temp >= 0) temp++;
  return temp;
}

/* Generate a double from 0.0 to 1.0 */
double gen_uniform_double()
{
  return drand48();
}

int check_percent(int percent)
{
  int retval;
  retval=(drand48() < (double) (percent/100.0));
  percentcheck++;
  if (retval) numlocal++;
  return retval;
}

void printstats()
{
  printf("percentcheck=%d,numlocal=%d\n",percentcheck,numlocal);
}
