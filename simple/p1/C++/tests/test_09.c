#include <stdio.h>
#include <sys/types.h>
extern int64_t test_09(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f, int64_t g, int64_t h, int64_t i, int64_t j);

int test_function(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f, int64_t g, int64_t h, int64_t i, int64_t j)
{
  int64_t arg = 10 / a;
  switch(arg) {
  case 0: return a;
  case 1: return b;
  case 2: return c;
  case 3: return d;
  case 4: return e;
  case 5: return f;
  case 6: return g;
  case 7: return h;
  case 8: return i;
  case 9: return j;
  default:
    return 0;
  };
  return 0;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=1; i<12; i++)
    if (test_09(i,1,2,3,4,5,6,7,8,9)!=test_function(i,1,2,3,4,5,6,7,8,9))
      errors++;
    else
      success++;

  int total=success+errors;
  printf("success,%d\nerrors,%d\ntotal,%d\n",success*10/total,errors*10/total,total*10/total);

  return 0;
}
