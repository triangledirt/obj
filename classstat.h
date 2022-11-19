#ifndef obj_classstat_h
#define obj_classstat_h

#include "bit.h"

struct obj_classstat_t {
  long truepos;
  long trueneg;
  long falsepos;
  long falseneg;
};
typedef struct obj_classstat_t obj_classstat_t;

void obj_classstat_reset(obj_classstat_t *stat);
void obj_classstat_note(obj_classstat_t *stat, obj_bit_t guess, obj_bit_t actual);
void obj_classstat_print(obj_classstat_t *stat, long type);

double obj_classstat_f1(obj_classstat_t *stat);
double obj_classstat_phi(obj_classstat_t *stat);
double obj_classstat_precision(obj_classstat_t *stat);
double obj_classstat_sensitivity(obj_classstat_t *stat);

#endif
