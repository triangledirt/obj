#ifndef obj_movegene_h
#define obj_movegene_h

#include "bit.h"
#include "obj.h"

struct obj_movegene_t {
  long xoffset;
  long yoffset;
};

void obj_movegene_parse(struct obj_movegene_t *movegene, obj_t obj, long startbit);

#endif
