#ifndef obj_displaygene_h
#define obj_displaygene_h

#include "obj.h"

struct obj_displaygene_t {
  char face;
};

void obj_displaygene_parse(struct obj_displaygene_t *displaygene, long startbit, obj_t obj);

#endif
