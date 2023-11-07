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

void obj_morph_learn(obj_t obj[], long objsz, long type)
{
  /*  determine game gene start position based on which start pos maximizes scores  */
}

double obj_morph_score(obj_t obj, long type)
{
  obj_t objx = obj;
  struct obj_morphgene_t morphgene;
  struct obj_stat_t stat;
  obj_morphgene_parse(&morphgene, objx, MORPH_GENE);
  obj_morph1ticks(&objx, morphgene.game, OBJ);
  stat = obj_stat(objx);
  return (double) stat.ones / OBJ;
}
