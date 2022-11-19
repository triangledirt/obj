#include <stdio.h>
#include "bit.h"
#include "class.h"
#include "coin.h"
#include "obj.h"
#include "sum.h"

static double fitness[OBJ_CLASS_TYPE];
static obj_t ideal[OBJ_CLASS_TYPE];
static obj_bool_t once = obj_bool_false;

static void init();

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < OBJ_CLASS_TYPE; type++) {
      fitness[type] = 0.0;
      obj_randomize(&ideal[type]);
    }
    once = obj_bool_true;
  }
}

void obj_sum_learn(obj_t obj[], long objsz, long type)
{
  long i;
  long bit;
  long onecount[OBJ];
  long thresh = objsz / 8;
  double tot = 0;
  init();
  for (bit = 1; bit < OBJ; bit++)
    onecount[bit] = 0;
  for (i = 0; i < objsz; i++) {
    if (obj_coin_toss())
      continue;
    if (obj_getclass(obj[i]))
      for (bit = 1; bit < OBJ; bit++)
        onecount[bit] += obj_getattr(obj[i], bit);
  }
  for (bit = 1; bit < OBJ; bit++)
    if (onecount[bit] > thresh) {
      obj_setattr(&ideal[type], bit, 1);
    } else {
      obj_setattr(&ideal[type], bit, 0);
    }
  for (i = 0; i < objsz; i++)
    if (obj_coin_toss())
      tot += obj_compare(ideal[type], obj[i]);
  fitness[type] = tot / (objsz / 2);
#if OBJ_VERBOSE
  printf("type%02ld ideal sum ", type);
  obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double obj_sum_score(obj_t obj, long type)
{
  init();
  return obj_compare(obj, ideal[type]);
}
