#include "selfgene.h"

void obj_selfgene_parse(struct obj_selfgene_t *selfgene, obj_t obj, long start)
{
  selfgene->color = obj_getattr(obj, start);
  selfgene->extrovert = obj_getattr(obj, start + 1);
  selfgene->narcissist = obj_getattr(obj, start + 2);
  selfgene->racist = obj_getattr(obj, start + 3);
}
