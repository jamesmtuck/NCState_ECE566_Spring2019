#include <stdio.h>
#include <sys/types.h>
extern int test_14(int, int y);

int test_function(int x, int y)
{
  return x||y;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-2; i<5; i++)
    if (test_14(i,0)!=test_function(i,0))
	  errors++;
	else
	  success++;

  for (i=-2; i<2; i++)
    if (test_14(i,1)!=test_function(i,1))
	  errors++;
	else
	  success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
