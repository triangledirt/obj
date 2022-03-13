#include <time.h>
#include "timer.h"

static struct timeval start;
static struct timeval stop;

void timer_start()
{
  gettimeofday(&start, 0);
}

long timer_stop()
{
  gettimeofday(&stop, 0);
  return stop.tv_usec - start.tv_usec;
}
