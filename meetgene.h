#ifndef obj_meetgene_h
#define obj_meetgene_h

#include "obj.h"

struct obj_meetgene_t {
  long startbit;
  long length;
};

void obj_meetgene_parse(struct obj_meetgene_t *meetgene, long startbit, obj_t obj);

#endif