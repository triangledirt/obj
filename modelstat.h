#ifndef obj_modelstat_h
#define obj_modelstat_h

#include "bit.h"

struct obj_modelstat_t {
  long ticks;
  long talks;
  long swaps;
};

void obj_modelstat_reset(struct obj_modelstat_t *stat);
/*  void obj_modelstat_note(struct obj_modelstat_t *stat, obj_bit_t guess, obj_bit_t actual);  */
void obj_modelstat_print(struct obj_modelstat_t *stat, long type);

#endif
