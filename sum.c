#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "obj.h"
#include "sum.h"

static double fitness[CASE_TYPE];
static case_obj_t ideal[CASE_TYPE];
static case_bool_t once = case_bool_false;

static void init();

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < CASE_TYPE; type++) {
      fitness[type] = 0.0;
      case_obj_randomize(&ideal[type]);
    }
    once = case_bool_true;
  }
}

void sum_learn(case_obj_t obj[], long objsz, long type)
{
  long i;
  long bit;
  long onecount[CASE_OBJ];
  long thresh = objsz / 8;
  double tot = 0;
  init();
  for (bit = 1; bit < CASE_OBJ; bit++)
    onecount[bit] = 0;
  for (i = 0; i < objsz; i++) {
    if (coin_toss())
      continue;
    if (case_obj_getclass(obj[i]))
      for (bit = 1; bit < CASE_OBJ; bit++)
        onecount[bit] += case_obj_getattr(obj[i], bit);
  }
  for (bit = 1; bit < CASE_OBJ; bit++)
    if (onecount[bit] > thresh) {
      case_obj_setattr(&ideal[type], bit, 1);
    } else {
      case_obj_setattr(&ideal[type], bit, 0);
    }
  for (i = 0; i < objsz; i++)
    if (coin_toss())
      tot += case_obj_compare(ideal[type], obj[i]);
  fitness[type] = tot / (objsz / 2);
#if CASE_VERBOSE
  printf("type%02ld ideal sum ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double sum_score(case_obj_t obj, long type)
{
  init();
  return case_obj_compare(obj, ideal[type]);
}
