#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "filt.h"
#include "obj.h"

#define ACTS 4

static case_bit_t once = 0;
static case_obj_t one[32];
static case_obj_t onesv[32];
static double fitness[32];
static double fitnesssv[32];
static case_obj_t zero[32];
static case_obj_t zerosv[32];

static void calcfit(case_obj_t obj[], long objsz, long type);
static void initonce();
static void mutate(case_obj_t *obj1, case_obj_t *obj2);
static void restore(long type);
static void save(long type);
static double score(case_obj_t obj, long type);

void calcfit(case_obj_t obj[], long objsz, long type)
{
  long i;
  case_obj_t o;
  double s = 0.0;
  for (i = 0; i < objsz; i++)
    if (coin_toss()) {
      o = obj[i];
      s += score(o, type);
    }
  fitness[type] = s / (objsz / 2);
}

case_bit_t filt_classify(case_obj_t obj, long type)
{
  initonce();
  return score(obj, type) > (0.75 * fitness[type]);
}

double filt_classifydouble(case_obj_t obj, long type)
{
  initonce();
  return score(obj, type);
}

void filt_learn(case_obj_t obj[], long objsz, long type)
{
  long bit;
  case_obj_t o;
  long act;
  initonce();
  for (act = 0; act < ACTS; act++) {
    calcfit(obj, objsz, type);
    save(type);
    mutate(&one[type], &zero[type]);
    calcfit(obj, objsz, type);
    if (fitnesssv[type] > fitness[type])
      restore(type);
  }
#if CASE_VERBOSE
  printf("type%ld filtr ft1 ", type);
  case_obj_print(one[type]);
  printf(" %0.3f\ntype%ld filtr ft0 ", fitness[type], type);
  case_obj_print(zero[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

void initonce()
{
  long type;
  if (!once) {
    for (type = 0; type < 32; type++) {
      case_obj_clear(&one[type]);
      case_obj_clear(&zero[type]);
    }
    once = 1;
  }
}

void mutate(case_obj_t *obj1, case_obj_t *obj2)
{
  case_obj_t *o1;
  case_obj_t *o2;
  long bit;
  case_bit_t val;
  bit = random() % 32;
  case_bit_randomize(&val);
  if (coin_toss()) {
    o1 = obj1;
    o2 = obj2;
  } else {
    o1 = obj2;
    o2 = obj1;
  }
  case_obj_setattr(o1, bit, val);
  case_obj_setattr(o2, bit, 0);
}

void restore(long type)
{
  one[type] = onesv[type];
  zero[type] = zerosv[type];
  fitness[type] = fitnesssv[type];
}

void save(long type)
{
  onesv[type] = one[type];
  zerosv[type] = zero[type];
  fitnesssv[type] = fitness[type];
}

double score(case_obj_t obj, long type)
{
  long bit;
  long onetot = 0;
  long zerotot = 0;
  long onematch = 0;
  long zeromatch = 0;
  case_bit_t onebit;
  case_bit_t zerobit;
  case_bit_t objbit;
  for (bit = 1; bit < 32; bit++) {
    onebit = case_obj_getattr(one[type], bit);
    zerobit = case_obj_getattr(zero[type], bit);
    onetot += onebit;
    zerotot += zerobit;
    objbit = case_obj_getattr(obj, bit);
    if (onebit && objbit)
      onematch++;
    if (zerobit && !objbit)
      zeromatch++;
  }
  return (double) (onematch + zeromatch) / (1 + onetot + zerotot);
}
