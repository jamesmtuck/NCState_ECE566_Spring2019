#include <stdio.h>

#define ITERS 1500

int* a[80000*2];

int main()
{
  double second();
  double time;
  int **b;
  long int c;
  long i,j,k,l,secx;
  /* Load L2 cache */
  for (i=0;i<80000*2;i++){
    a[i]=(int *)&a[i+1];
  }

  time=second();
  /* loop 1500 times */
  for (i=0;i<ITERS;i++) {
    b = (int **)a[7];
    /* Load from L2 at 64 byte boundary */
    for(k=0;k<5000;k+=1) {
      b = (int **) b[64/sizeof(int**)];
    }
  }
  time=second()-time;
  fprintf(stderr, "Time is %g\n",time);
  fprintf(stderr, "L2 Latency (ns) is %g\n",(time)/(5000*ITERS)*1e9);
  fprintf(stderr, "L2 BW is %f MB/sec\n",(5000*ITERS*64)/(time*1000000));
  fprintf(stderr, "b is %ld\n",(long)b);
  return 0;
}
