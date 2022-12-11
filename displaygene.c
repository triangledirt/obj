#include "displaygene.h"

void obj_displaygene_parse(struct obj_displaygene_t *displaygene, obj_t obj, long startbit)
{
  displaygene->face = obj_getnum(obj, startbit, 8);
}
