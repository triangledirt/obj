#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "obj.h"
#include "sum.h"

static double fitness[32];
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void initonce();

void initonce()
{
  long type;
  if (!once) {
    for (type = 0; type < 32; type++) {
      fitness[type] = 0.0;
      case_obj_randomize(&ideal[type]);
    }
    once = 1;
  }
}

case_bit_t sum_classify(case_obj_t obj, long type)
{
  initonce();
  return case_obj_comparetypes(obj, ideal[type]) > (0.75 * fitness[type]);
}

double sum_classifydouble(case_obj_t obj, long type)
{
  initonce();
  return case_obj_comparetypes(obj, ideal[type]);
}

void sum_learn(case_obj_t obj[], long objsz, long type)
{
  long i;
  long bit;
  case_bit_t val;
  case_obj_t o;
  long onecount[32];
  long thresh = objsz / 8;
  case_bit_t class;
  double tot = 0;
  initonce();
  for (bit = 1; bit < 32; bit++)
    onecount[bit] = 0;
  for (i = 0; i < objsz; i++) {
    if (coin_toss())
      continue;
    o = obj[i];
    class = case_obj_getclass(o);
    if (class)
      for (bit = 1; bit < 32; bit++) {
        val = case_obj_getattr(o, bit);
        if (val)
          onecount[bit]++;
      }
  }
  for (bit = 1; bit < 32; bit++)
    if (onecount[bit] > thresh) {
      case_obj_setattr(&ideal[type], bit, 1);
    } else {
      case_obj_setattr(&ideal[type], bit, 0);
    }
  for (i = 0; i < objsz; i++)
    if (coin_toss())
      tot += case_obj_comparetypes(ideal[type], obj[i]);
  fitness[type] = tot / (objsz / 2);
#if CASE_VERBOSE
  printf("type%ld ideal sum ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}
