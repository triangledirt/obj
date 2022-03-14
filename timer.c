#include <sys/time.h>
#include <time.h>
#include "timer.h"

static struct timeval start;
static struct timeval stop;
static long total;

void timer_start(long totalus)
{
  total = totalus;
  gettimeofday(&start, 0);
}

long timer_stop()
{
  gettimeofday(&stop, 0);
  return total + (stop.tv_usec - start.tv_usec);
}
