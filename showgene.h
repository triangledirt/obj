#ifndef obj_showgene_h
#define obj_showgene_h

#include "obj.h"

struct obj_showgene_t {
  char face;
};

void obj_showgene_parse(struct obj_showgene_t *showgene, obj_t obj, long start);

#endif
