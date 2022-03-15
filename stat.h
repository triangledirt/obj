#ifndef case_stat_h
#define case_stat_h

#include "bit.h"

struct case_stat_t {
  long truepos;
  long falsepos;
  long trueneg;
  long falseneg;
};
typedef struct case_stat_t case_stat_t;

void case_stat_reset(case_stat_t *stat);
void case_stat_noteclasses(case_stat_t *stat, case_bit_t guess, case_bit_t actual);

double case_stat_fmeasure(case_stat_t *stat);
double case_stat_precision(case_stat_t *stat);
double case_stat_recall(case_stat_t *stat);

void case_stat_print(case_stat_t *stat);

#endif
