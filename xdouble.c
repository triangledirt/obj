#include <stdlib.h>
#include "xdouble.h"

static void calc(struct obj_xdouble_t *xdouble);

void calc(struct obj_xdouble_t *xdouble)
{
  double tot = 0.0;
  long i;
  for (i = 0; i < OBJ_XDOUBLE_CNT; i++)
    tot += xdouble->num[i];
  xdouble->avg = tot / OBJ_XDOUBLE_CNT;
}

double obj_xdouble_avg(struct obj_xdouble_t *xdouble)
{
  if (xdouble->notecnt >= (OBJ_XDOUBLE_CNT / 2)) {
    calc(xdouble);
    xdouble->notecnt = 0;
  }
  return xdouble->avg;
}

void obj_xdouble_init(struct obj_xdouble_t *xdouble)
{
  long i;
  for (i = 0; i < OBJ_XDOUBLE_CNT; i++)
    xdouble->num[i] = 0.0;
  xdouble->notecnt = 0;
  xdouble->avg = 0.0;
}

void obj_xdouble_note(struct obj_xdouble_t *xdouble, double num)
{
  xdouble->num[random() % OBJ_XDOUBLE_CNT] = num;
  xdouble->notecnt++;
}
