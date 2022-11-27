#ifndef obj_syncstat_h
#define obj_syncstat_h

#include "bit.h"

struct obj_syncstat_t {
};

void obj_syncstat_reset(struct obj_syncstat_t *stat);
/*  void obj_syncstat_note(struct obj_syncstat_t *stat, obj_bit_t guess, obj_bit_t actual);  */
void obj_syncstat_print(struct obj_syncstat_t *stat, long type);

#endif
