#include <fcntl.h>
#include <stdio.h>

int dealwithargs(int argc, char *argv[])
{
  int level;

  if (argc > 1)
    level = atoi(argv[1]);
  else
    level = 1024;

  return level;
}

