#ifndef obj_classstat_h
#define obj_classstat_h

#include "bit.h"

struct obj_classstat_t {
  long truepos;
  long trueneg;
  long falsepos;
  long falseneg;
};

void obj_classstat_reset(struct obj_classstat_t *stat);
void obj_classstat_note(struct obj_classstat_t *stat, obj_bit_t guess, obj_bit_t actual);
void obj_classstat_print(struct obj_classstat_t *stat, long type);

double obj_classstat_f1(struct obj_classstat_t *stat);
double obj_classstat_phi(struct obj_classstat_t *stat);
double obj_classstat_precision(struct obj_classstat_t *stat);
double obj_classstat_sensitivity(struct obj_classstat_t *stat);

#endif
