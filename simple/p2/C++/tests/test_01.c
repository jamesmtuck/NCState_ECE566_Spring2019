#include <stdio.h>
#include <sys/types.h>
extern int test_01(int,int,int);

int test_function(int a, int b, int c)
{
  return a+b+c;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=5; i<10; i++)
    for (j=100; j<103; j++)
      for (k=-10; k<-8; k++)
	if (test_01(i,j,k)!=test_function(i,j,k))
	  errors++;
	else
	  success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
