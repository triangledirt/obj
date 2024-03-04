#ifndef obj_lexis_h
#define obj_lexis_h

#define OBJ_LEXIS_PHRASE 256

#include "phrase.h"

struct obj_lexis_t {
  struct obj_phrase_t phrase[OBJ_LEXIS_PHRASE];
};

void obj_lexis_init(struct obj_lexis_t *lexis, long meshsz);

#endif
