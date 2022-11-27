#include "displaygene.h"

void obj_displaygene_parse(struct obj_displaygene_t *displaygene, long startbit, obj_t obj)
{
  displaygene->face = obj_getnum(obj, startbit, 8);
}
