#include <stdio.h>
#include <sys/types.h>

extern int test_07(int,int);

int test_function(int x, int y)
{
  int j=0;

  while (j<10)
    {
      if (x > y)
	x = x-y;
      else
	x = x+y;

      j++;
    }

  return x;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-2,j=10; i<20; i++,j--)
    if (test_07(i,j)!=test_function(i,j))
	  errors++;
	else
	  success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
