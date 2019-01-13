extern int nbody;
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
int dealwithargs(int argc, char *argv[])
{
  int level;

  if (argc > 1)
    nbody = atoi(argv[1]);
  else
    nbody = 2048;

  return level;
  
}
