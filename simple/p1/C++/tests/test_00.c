#include <stdio.h>
#include <sys/types.h>
extern int64_t test_00();

int test_function()
{
  return 16;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  if (test_00()!=test_function())
    errors++;
  else
    success++;

  int total=success+errors;
  printf("success,%d\nerrors,%d\ntotal,%d\n",success*10/total,errors*10/total,total*10/total);

  return 0;
}
