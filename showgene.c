#include "showgene.h"

void obj_showgene_parse(struct obj_showgene_t *showgene, obj_t obj, long start)
{
  showgene->face = obj_num(obj, start, 8);
}
