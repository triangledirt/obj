#include <stdlib.h>
#include "past.h"

static void calcavg(past_t *past);

void calcavg(past_t *past)
{
  double tot = 0.0;
  long i;
  for (i = 0; i < NUM; i++) {
    tot += past->num[i];
  }
  past->avg = tot / NUM;
}

double past_avg(past_t *past)
{
  if (past->notecnt >= (NUM / 2)) {
    calcavg(past);
    past->notecnt = 0;
  }
  return past->avg;
}

void past_init(past_t *past)
{
  long i;
  for (i = 0; i < NUM; i++) {
    past->num[i] = 0.0;
  }
  past->notecnt = 0;
  past->avg = 0.0;
}

void past_note(past_t *past, double num)
{
  past->num[random() % NUM] = num;
  past->notecnt++;
}
