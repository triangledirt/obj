#include "persongene.h"

void obj_persongene_parse(struct obj_persongene_t *persongene, obj_t obj, long startbit)
{
  persongene->extrovert = obj_getattr(obj, startbit);
  persongene->racist = obj_getattr(obj, startbit + 1);
}
