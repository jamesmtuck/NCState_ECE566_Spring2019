#include <stdio.h>
#include <sys/types.h>
extern int test_11(float);

int test_function(float x)
{
  int j;
  int i;
  int sum;
  sum=0;
  for(j=0; j<10; j++)
    for(i=0; i<5; i++)
      sum = sum + x;

  return sum;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-2; i<20; i++)
	if (test_11(i)!=test_function(i))
	  errors++;
	else
	  success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
