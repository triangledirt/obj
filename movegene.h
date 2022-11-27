#ifndef obj_movegene_h
#define obj_movegene_h

#include "bit.h"
#include "obj.h"

struct obj_movegene_t {
  obj_bit_t xoffset;
  obj_bit_t yoffset;
};

void obj_movegene_parse(struct obj_movegene_t *movegene, long startbit, obj_t obj);

#endif
