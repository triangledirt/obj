#ifndef case_step_h
#define case_step_h

#include "bit.h"

struct case_step_t {
  long truepos;
  long falsepos;
  long trueneg;
  long falseneg;
};
typedef struct case_step_t case_step_t;

void case_step_reset(case_step_t *step);
void case_step_noteclasses(case_step_t *step, case_bit_t guess, case_bit_t actual);

double case_step_fmeasure(case_step_t *step);
double case_step_precision(case_step_t *step);
double case_step_recall(case_step_t *step);

void case_step_print(case_step_t *step);

#endif
