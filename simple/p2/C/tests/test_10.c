#include <stdio.h>
#include <sys/types.h>
extern float test_10(float);

float test_function(float x)
{
  int j;
  float sum;
  sum=0.0;
  for(j=0; j<10; j++)
    sum = sum + x;

  return sum;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-2; i<20; i++)
	if (test_10(i)!=test_function(i))
	  errors++;
	else
	  success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
