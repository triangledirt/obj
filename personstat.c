#include <math.h>
#include <stdio.h>
#include "personstat.h"

double obj_personstat_f1(obj_personstat_t *stat)
{
  return (double) (2 * stat->truepos) / (1 + (2 * stat->truepos) + stat->falsepos + stat->falseneg);
}

void obj_personstat_note(obj_personstat_t *stat, obj_bit_t guess, obj_bit_t actual)
{
  if ((0 == guess) && (0 == actual)) {
    stat->trueneg++;
  } else if ((0 == guess) && (1 == actual)) {
    stat->falseneg++;
  } else if ((1 == guess) && (0 == actual)) {
    stat->falsepos++;
  } else {
    stat->truepos++;
  }
}

double obj_personstat_phi(obj_personstat_t *stat)
{
  return (stat->truepos * stat->trueneg - stat->falsepos * stat->falseneg) / (1 + sqrt((stat->truepos + stat->falsepos) * (stat->truepos + stat->falseneg) * (stat->trueneg + stat->falsepos) * (stat->trueneg + stat->falseneg)));
}

double obj_personstat_precision(obj_personstat_t *stat)
{
  return (double) stat->truepos / (1 + stat->truepos + stat->falsepos);
}

void obj_personstat_print(obj_personstat_t *stat, long type)
{
  double phi;
  double f1;
  double precision;
  double sensitivity;
  phi = obj_personstat_phi(stat);
  f1 = obj_personstat_f1(stat);
  precision = obj_personstat_precision(stat);
  sensitivity = obj_personstat_sensitivity(stat);
  printf("type%02ld stats     phi=%0.3f f1=%0.3f prec=%0.3f sens=%0.3f tpos=%ld tneg=%ld fpos=%ld fneg=%ld\n", type, phi, f1, precision, sensitivity, stat->truepos, stat->trueneg, stat->falsepos, stat->falseneg);
}

double obj_personstat_sensitivity(obj_personstat_t *stat)
{
  return (double) stat->truepos / (1 + stat->truepos + stat->falseneg);
}

void obj_personstat_reset(obj_personstat_t *stat)
{
  stat->truepos = 0;
  stat->trueneg = 0;
  stat->falsepos = 0;
  stat->falseneg = 0;
}
