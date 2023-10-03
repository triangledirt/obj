#include <sys/time.h>
#include <time.h>
#include "timer.h"

static struct timeval start;
static struct timeval stop;
static long inittime;

void obj_timer_start(long time)
{
  inittime = time;
  gettimeofday(&start, NULL);
}

long obj_timer_stop()
{
  gettimeofday(&stop, NULL);
  return inittime + (stop.tv_usec - start.tv_usec);
}
