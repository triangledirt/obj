#include "talkgene.h"

void obj_talkgene_parse(struct obj_talkgene_t *talkgene, obj_t obj, long start)
{
  talkgene->send.start = obj_num(obj, start, 6);
  talkgene->send.length = obj_num(obj, start + 6, 6);
  talkgene->receive.start = obj_num(obj, start + 12, 6);
  talkgene->receive.length = obj_num(obj, start + 18, 6);
}
