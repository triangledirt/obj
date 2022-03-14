#ifndef step_h
#define step_h

#include "bit.h"

struct step_t {
  long tp;
  long fp;
  long tn;
  long fn;
};
typedef struct step_t step_t;

void step_reset(step_t *step);
void step_noteclasses(step_t *step, case_bit_t guess, case_bit_t actual);

double step_fmeasure(step_t *step);
double step_precision(step_t *step);
double step_recall(step_t *step);

void step_print(step_t *step);

#endif
