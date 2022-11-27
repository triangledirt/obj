#include "movegene.h"

void obj_movegene_parse(struct obj_movegene_t *movegene, long startbit, obj_t obj)
{
  movegene->xoffset = obj_getnum(obj, startbit, 1);
  movegene->yoffset = obj_getnum(obj, startbit + 1, 1);
}
