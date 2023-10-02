#ifndef obj_meetgene_h
#define obj_meetgene_h

#include "obj.h"
#include "segment.h"

struct obj_meetgene_t {
  struct obj_segment_t send;
  struct obj_segment_t receive;
};

void obj_meetgene_parse(struct obj_meetgene_t *meetgene, obj_t obj, long start);

#endif
