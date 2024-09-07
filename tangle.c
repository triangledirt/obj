#include "lexis.h"
#include "tangle.h"

static double fitness[OBJ_TYPE];
static struct obj_lexis_t lexis;
static enum obj_bool_t once = obj_bool_false;

static void init();

void init()
{
  if (!once) {
    obj_lexis_init(&lexis, OBJ_PHRASE_MESH);
    once = obj_bool_true;
  }
}

double obj_tangle_fit(long type)
{
  init();
  return fitness[type];
}

void obj_tangle_learn(obj_t obj[], long objsz, long type)
{
  init();
}

double obj_tangle_score(obj_t obj, long type)
{
  init();
  return 0.0;
}
