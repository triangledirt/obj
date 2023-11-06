#ifndef obj_morphgene_h
#define obj_morphgene_h

#include "obj.h"
#include "slice.h"

struct obj_morphgene_t {
  long game;
};

void obj_morphgene_parse(struct obj_morphgene_t *morphgene, obj_t obj, long start);

#endif
