#include "meetgene.h"

void obj_meetgene_parse(struct obj_meetgene_t *meetgene, long startbit, obj_t obj)
{
  meetgene->startbit = obj_getnum(obj, startbit, 6);
  meetgene->length = obj_getnum(obj, startbit + 6, 6);
}