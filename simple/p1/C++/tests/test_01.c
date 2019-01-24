#include <stdio.h>
#include <sys/types.h>
extern int64_t test_01(int64_t,int64_t,int64_t);

int test_function(int a, int b, int c)
{
  return a+b+c;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=0; i<10; i++)
    for (j=100; j<120; j++)
      for (k=-10; k<0; k++)
	if (test_01(i,j,k)!=test_function(i,j,k))
	  errors++;
	else
	  success++;

  int total=success+errors;
  printf("success,%d\nerrors,%d\ntotal,%d\n",success*10/total,errors*10/total,total*10/total);

  return 0;
}
