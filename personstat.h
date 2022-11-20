#ifndef obj_personstat_h
#define obj_personstat_h

#include "bit.h"

struct obj_personstat_t {
};
typedef struct obj_personstat_t obj_personstat_t;

void obj_personstat_reset(obj_personstat_t *stat);
void obj_personstat_note(obj_personstat_t *stat, obj_bit_t guess, obj_bit_t actual);
void obj_personstat_print(obj_personstat_t *stat, long type);

double obj_personstat_f1(obj_personstat_t *stat);

#endif
