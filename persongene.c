#include "persongene.h"

void obj_persongene_parse(struct obj_persongene_t *persongene, obj_t obj, long start)
{
  persongene->extrovert = obj_getattr(obj, start);
  persongene->racist = obj_getattr(obj, start + 1);
}
