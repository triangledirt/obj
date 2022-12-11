#ifndef obj_persongene_h
#define obj_persongene_h

#include "bit.h"
#include "obj.h"

struct obj_persongene_t {
  obj_bit_t extrovert;
  obj_bit_t racist;
};

void obj_persongene_parse(struct obj_persongene_t *persongene, obj_t obj, long startbit);

#endif
