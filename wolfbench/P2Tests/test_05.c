#include <stdio.h>
#include <sys/types.h>
extern int test_05(int);

int test_function(int x)
{
  int j;
  j=0;
  while(x>0) {
    j++;
    x--;
  }
  return j;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-2; i<20; i++)
	if (test_05(i)!=test_function(i))
	  errors++;
	else
	  success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
