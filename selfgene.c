#include "selfgene.h"

void obj_selfgene_parse(struct obj_selfgene_t *selfgene, obj_t obj, long start)
{
  selfgene->color = obj_attr(obj, start);
  selfgene->extrovert = obj_attr(obj, start + 1);
  selfgene->narcissist = obj_attr(obj, start + 2);
  selfgene->racist = obj_attr(obj, start + 3);
}
