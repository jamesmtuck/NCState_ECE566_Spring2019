#include <stdio.h>
#include <sys/types.h>
extern int64_t test_08(int64_t,int64_t,int64_t,int64_t);

int test_function(int x, int y, int z, int w)
{
  return 16;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=1; i<5; i++)
    for (j=1; j<5; j++)
      for (k=1; k<20; k+=7)
	if (test_08(i,j,k,3)!=test_function(i,j,k,3))
	  errors++;
	else
	  success++;

  int total=success+errors;
  printf("success,%d\nerrors,%d\ntotal,%d\n",success*10/total,errors*10/total,total*10/total);

  return 0;
}
