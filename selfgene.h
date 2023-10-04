#ifndef obj_selfgene_h
#define obj_selfgene_h

#include "bit.h"
#include "obj.h"

struct obj_selfgene_t {
  obj_bit_t color;
  obj_bit_t extrovert;
  obj_bit_t narcissist;
  obj_bit_t racist;
};

void obj_selfgene_parse(struct obj_selfgene_t *selfgene, obj_t obj, long start);

#endif
