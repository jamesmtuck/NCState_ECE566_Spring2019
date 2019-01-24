#include <stdio.h>
#include <sys/types.h>
extern int64_t test_06(int64_t,int64_t,int64_t,int64_t);


int test_function(int x, int y, int z, int w)
{
  switch(x) {
  case 0: return x;
  case 1: return y;
  case 2: return z;
  case 3: return w;
  default:
    return 0;
  }
}

int main()
{  
  int i, j, k, l;
  int errors=0;
  int success=0;

  j = 2;
  k = 3;
  l = 4;

  for (i=-1; i<6; i++)
    if (test_06(i,j,k,l)!=test_function(i,j,k,l))
      errors++;
    else
      success++;

  int total=success+errors;
  printf("success,%d\nerrors,%d\ntotal,%d\n",success*10/total,errors*10/total,total*10/total);

  return 0;
}
