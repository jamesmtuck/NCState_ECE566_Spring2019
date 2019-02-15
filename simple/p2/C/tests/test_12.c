#include <stdio.h>
#include <sys/types.h>
extern int *test_12(int *, int *);

int *test_function(int *x, int *y)
{
  if( x && y && *x > *y)
    return x;
  else
    return y;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-2,j=10; i<20; i++, j--)
    if (test_12(&i,&j)!=test_function(&i,&j))
	  errors++;
	else
	  success++;

  i = 5;
  if (test_12(&i,NULL) != NULL)
    errors++;
  else 
    success++;

  i = 5;
  if (test_12(NULL,&i) != &i)
    errors++;
  else 
    success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
