#include <stdio.h>
#include <sys/types.h>
extern int64_t test_02(int64_t);

int test_function(int a)
{
  return a;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=0; i<10; i++)
    if (test_02(i)!=test_function(i))
      errors++;
    else
      success++;

  int total=success+errors;
  printf("success,%d\nerrors,%d\ntotal,%d\n",success*10/total,errors*10/total,total*10/total);

  return 0;
}
