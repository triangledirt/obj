#include <math.h>
#include <stdio.h>
#include "stat.h"

double case_stat_f1(case_stat_t *stat)
{
  return (double) (2 * stat->truepos) / (1 + (2 * stat->truepos) + stat->falsepos + stat->falseneg);
}

void case_stat_note(case_stat_t *stat, case_bit_t guess, case_bit_t actual)
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

double case_stat_phi(case_stat_t *stat)
{
  return (stat->truepos * stat->trueneg - stat->falsepos * stat->falseneg) / (1 + sqrt((stat->truepos + stat->falsepos) * (stat->truepos + stat->falseneg) * (stat->trueneg + stat->falsepos) * (stat->trueneg + stat->falseneg)));
}

double case_stat_precision(case_stat_t *stat)
{
  return (double) stat->truepos / (1 + stat->truepos + stat->falsepos);
}

void case_stat_print(case_stat_t *stat, long type)
{
  double phi;
  double f1;
  double precision;
  double sensitivity;
  phi = case_stat_phi(stat);
  f1 = case_stat_f1(stat);
  precision = case_stat_precision(stat);
  sensitivity = case_stat_sensitivity(stat);
  printf("type%02ld stats     phi=%0.3f f1=%0.3f prec=%0.3f sens=%0.3f tpos=%ld tneg=%ld fpos=%ld fneg=%ld\n", type, phi, f1, precision, sensitivity, stat->truepos, stat->trueneg, stat->falsepos, stat->falseneg);
}

double case_stat_sensitivity(case_stat_t *stat)
{
  return (double) stat->truepos / (1 + stat->truepos + stat->falseneg);
}

void case_stat_reset(case_stat_t *stat)
{
  stat->truepos = 0;
  stat->trueneg = 0;
  stat->falsepos = 0;
  stat->falseneg = 0;
}
