#include <stdlib.h>
#include "xdouble.h"

static void calc(xdouble_t *xdouble);

void calc(xdouble_t *xdouble)
{
  double tot = 0.0;
  long i;
  for (i = 0; i < XDOUBLE_CNT; i++)
    tot += xdouble->num[i];
  xdouble->avg = tot / XDOUBLE_CNT;
}

double xdouble_avg(xdouble_t *xdouble)
{
  if (xdouble->notecnt >= (XDOUBLE_CNT / 2)) {
    calc(xdouble);
    xdouble->notecnt = 0;
  }
  return xdouble->avg;
}

void xdouble_init(xdouble_t *xdouble)
{
  long i;
  for (i = 0; i < XDOUBLE_CNT; i++)
    xdouble->num[i] = 0.0;
  xdouble->notecnt = 0;
  xdouble->avg = 0.0;
}

void xdouble_note(xdouble_t *xdouble, double num)
{
  xdouble->num[random() % XDOUBLE_CNT] = num;
  xdouble->notecnt++;
}
