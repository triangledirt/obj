#include "morphgene.h"

void obj_morphgene_parse(struct obj_morphgene_t *morphgene, obj_t obj, long start)
{
  morphgene->game = obj_num(obj, start, 8);
}
