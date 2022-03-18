#include <stdio.h>
#include "stat.h"

double case_stat_fmeasure(case_stat_t *stat)
{
  double precision;
  double recall;
  double fmeasure;
  precision = case_stat_precision(stat);
  recall = case_stat_recall(stat);
  fmeasure = (2 * precision * recall) / (precision + recall);
  return fmeasure;
}

void case_stat_noteclasses(case_stat_t *stat, case_bit_t guess, case_bit_t actual)
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

double case_stat_precision(case_stat_t *stat)
{
  return (double) stat->truepos / (1 + stat->truepos + stat->falsepos);
}

void case_stat_print(case_stat_t *stat)
{
  double precision;
  double recall;
  double fmeasure;
  precision = case_stat_precision(stat);
  recall = case_stat_recall(stat);
  fmeasure = case_stat_fmeasure(stat);
  printf("                 fmeasure=%0.3f precision=%0.3f recall=%0.3f truepos=%ld falsepos=%ld trueneg=%ld falseneg=%ld\n", fmeasure, precision, recall, stat->truepos, stat->falsepos, stat->trueneg, stat->falseneg);
}

double case_stat_recall(case_stat_t *stat)
{
  return (double) stat->truepos / (1 + stat->truepos + stat->falseneg);
}

void case_stat_reset(case_stat_t *stat)
{
  stat->truepos = 0;
  stat->falsepos = 0;
  stat->trueneg = 0;
  stat->falseneg = 0;
}
