#include "persongene.h"

void obj_persongene_parse(struct obj_persongene_t *persongene, obj_t obj, long start)
{
  persongene->color = obj_getattr(obj, start);
  persongene->extrovert = obj_getattr(obj, start + 1);
  persongene->narcissist = obj_getattr(obj, start + 2);
  persongene->racist = obj_getattr(obj, start + 3);
}
