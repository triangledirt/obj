#include <stdio.h>
#include "step.h"

double step_fmeasure(step_t *step)
{
  double precision;
  double recall;
  double fmeasure;
  precision = step_precision(step);
  recall = step_recall(step);
  fmeasure = (2 * precision * recall) / (precision + recall);
  return fmeasure;
}

void step_noteclasses(step_t *step, case_bit_t guess, case_bit_t actual)
{
  if ((0 == guess) && (0 == actual)) {
    step->tn++;
  } else if ((0 == guess) && (1 == actual)) {
    step->fn++;
  } else if ((1 == guess) && (0 == actual)) {
    step->fp++;
  } else {
    step->tp++;
  }
}

double step_precision(step_t *step)
{
  return (double) step->tp / (1 + step->tp + step->fp);
}

void step_print(step_t *step)
{
  double precision;
  double recall;
  double fmeasure;
  precision = step_precision(step);
  recall = step_recall(step);
  fmeasure = step_fmeasure(step);
  printf("fmeasure=%0.3f precision=%0.3f recall=%0.3f tp=%ld fp=%ld tn=%ld fn=%ld\n", fmeasure, precision, recall, step->tp, step->fp, step->tn, step->fn);
}

double step_recall(step_t *step)
{
  return (double) step->tp / (1 + step->tp + step->fn);
}

void step_reset(step_t *step)
{
  step->tp = 0;
  step->fp = 0;
  step->tn = 0;
  step->fn = 0;
}
