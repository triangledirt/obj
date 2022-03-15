#include <stdio.h>
#include "step.h"

double case_step_fmeasure(case_step_t *step)
{
  double precision;
  double recall;
  double fmeasure;
  precision = case_step_precision(step);
  recall = case_step_recall(step);
  fmeasure = (2 * precision * recall) / (precision + recall);
  return fmeasure;
}

void case_step_noteclasses(case_step_t *step, case_bit_t guess, case_bit_t actual)
{
  if ((0 == guess) && (0 == actual)) {
    step->trueneg++;
  } else if ((0 == guess) && (1 == actual)) {
    step->falseneg++;
  } else if ((1 == guess) && (0 == actual)) {
    step->falsepos++;
  } else {
    step->truepos++;
  }
}

double case_step_precision(case_step_t *step)
{
  return (double) step->truepos / (1 + step->truepos + step->falsepos);
}

void case_step_print(case_step_t *step)
{
  double precision;
  double recall;
  double fmeasure;
  precision = case_step_precision(step);
  recall = case_step_recall(step);
  fmeasure = case_step_fmeasure(step);
  printf("fmeasure=%0.3f precision=%0.3f recall=%0.3f truepos=%ld falsepos=%ld trueneg=%ld falseneg=%ld\n", fmeasure, precision, recall, step->truepos, step->falsepos, step->trueneg, step->falseneg);
}

double case_step_recall(case_step_t *step)
{
  return (double) step->truepos / (1 + step->truepos + step->falseneg);
}

void case_step_reset(case_step_t *step)
{
  step->truepos = 0;
  step->falsepos = 0;
  step->trueneg = 0;
  step->falseneg = 0;
}
