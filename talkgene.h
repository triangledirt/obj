#ifndef obj_talkgene_h
#define obj_talkgene_h

#include "obj.h"
#include "slice.h"

struct obj_talkgene_t {
  struct obj_slice_t send;
  struct obj_slice_t receive;
};

void obj_talkgene_parse(struct obj_talkgene_t *talkgene, obj_t obj, long start);

#endif
