#include <math.h>
#include <stdio.h>
#include "classstat.h"

double obj_classstat_f1(struct obj_classstat_t *stat)
{
  return (double) (2 * stat->truepos) / (1 + (2 * stat->truepos) + stat->falsepos + stat->falseneg);
}

void obj_classstat_note(struct obj_classstat_t *stat, obj_bit_t guess, obj_bit_t actual)
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

double obj_classstat_phi(struct obj_classstat_t *stat)
{
  return (stat->truepos * stat->trueneg - stat->falsepos * stat->falseneg) / (1 + sqrt((stat->truepos + stat->falsepos) * (stat->truepos + stat->falseneg) * (stat->trueneg + stat->falsepos) * (stat->trueneg + stat->falseneg)));
}

double obj_classstat_precision(struct obj_classstat_t *stat)
{
  return (double) stat->truepos / (1 + stat->truepos + stat->falsepos);
}

void obj_classstat_print(struct obj_classstat_t *stat, long type)
{
  double phi;
  double f1;
  double precision;
  double sensitivity;
  phi = obj_classstat_phi(stat);
  f1 = obj_classstat_f1(stat);
  precision = obj_classstat_precision(stat);
  sensitivity = obj_classstat_sensitivity(stat);
  printf("type%02ld stats     phi=%0.3f f1=%0.3f prec=%0.3f sens=%0.3f tpos=%ld tneg=%ld fpos=%ld fneg=%ld\n", type, phi, f1, precision, sensitivity, stat->truepos, stat->trueneg, stat->falsepos, stat->falseneg);
}

double obj_classstat_sensitivity(struct obj_classstat_t *stat)
{
  return (double) stat->truepos / (1 + stat->truepos + stat->falseneg);
}

void obj_classstat_reset(struct obj_classstat_t *stat)
{
  stat->truepos = 0;
  stat->trueneg = 0;
  stat->falsepos = 0;
  stat->falseneg = 0;
}
