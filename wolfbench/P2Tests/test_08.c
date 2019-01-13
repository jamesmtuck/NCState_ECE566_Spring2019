#include <stdio.h>
#include <sys/types.h>

extern int test_08(int,int);

int test_function(int x, int y)
{
  int j=0;

  if (x < 0)
    x = -x;
  else
    ;

  if (y < 0)
    y = -y;
  else 
    ;

  if (x > y && y > 0)
    {
      while (x > y)
	{
	  x = x - y;
	  j = j+1;
	}
    }
  else
    {
      j = -1;
    }

  return j;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=30,j=2; j<15; j+=1)
    {
      if (test_08(i,j)!=test_function(i,j))
	errors++;
      else
	success++;
    }

  if (test_08(10,20)!=test_function(10,20))
    errors++;
  else
    success++;

  if (test_08(-10,20)!=test_function(-10,20))
    errors++;
  else
    success++;

  if (test_08(20,-10)!=test_function(20,-10))
    errors++;
  else
    success++;

  if (test_08(-20,-10)!=test_function(-20,-10))
    errors++;
  else
    success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
