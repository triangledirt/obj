#include <stdio.h>
#include "bit.h"
#include "class.h"
#include "coin.h"
#include "morph.h"
#include "morphgene.h"
#include "obj.h"

#define MORPH_GENE 0

double obj_morph_fit(long type)
{
  return 0.0;
}

void obj_morph_learn(obj_t obj[], long objsz, long type) {}

double obj_morph_score(obj_t obj, long type)
{
  obj_t objx = obj;
  struct obj_morphgene_t morphgene;
  long count1;
  obj_morphgene_parse(&morphgene, objx, MORPH_GENE);
  obj_morphticks1(&objx, morphgene.game, OBJ);
  count1 = obj_count1s(objx);
  return (double) (count1 / OBJ);
}
