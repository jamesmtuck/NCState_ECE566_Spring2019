#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <time.h>

double second()
{
  double secx;
  struct timeval tv;

  gettimeofday(&tv,0);

  secx = tv.tv_usec;
  secx /= 1000000;

  secx += tv.tv_sec;

  return secx;
}
