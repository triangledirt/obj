#include "meetgene.h"

void obj_meetgene_parse(struct obj_meetgene_t *meetgene, obj_t obj, long start)
{
  meetgene->send.start = obj_getnum(obj, start, 6);
  meetgene->send.length = obj_getnum(obj, start + 6, 6);
  meetgene->receive.start = obj_getnum(obj, start + 12, 6);
  meetgene->receive.length = obj_getnum(obj, start + 18, 6);
}
