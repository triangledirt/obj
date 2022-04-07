#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "filt.h"
#include "obj.h"

static case_bool_t once = case_bool_false;
static case_obj_t one[CASE_TYPE];
static case_obj_t onesv[CASE_TYPE];
static double fitness[CASE_TYPE];
static double fitnesssv[CASE_TYPE];
static case_obj_t zero[CASE_TYPE];
static case_obj_t zerosv[CASE_TYPE];

static void calcfit(case_obj_t obj[], long objsz, long type);
static void init();
static void mutate(case_obj_t *obj1, case_obj_t *obj2);
static void restore(long type);
static void save(long type);
static double score(case_obj_t obj, long type);

void calcfit(case_obj_t obj[], long objsz, long type)
{
  long i;
  double s = 0.0;
  for (i = 0; i < objsz; i++)
    if (coin_toss())
      s += score(obj[i], type);
  fitness[type] = s / (objsz / 2);
}

void filt_learn(case_obj_t obj[], long objsz, long type)
{
  long bit;
  case_obj_t o;
  long act;
  init();
  for (act = 0; act < 4; act++) {
    calcfit(obj, objsz, type);
    save(type);
    mutate(&one[type], &zero[type]);
    calcfit(obj, objsz, type);
    if (fitnesssv[type] > fitness[type])
      restore(type);
  }
#if CASE_VERBOSE
  printf("type%02ld filtr ft1 ", type);
  case_obj_print(one[type]);
  printf(" %0.3f\ntype%02ld filtr ft0 ", fitness[type], type);
  case_obj_print(zero[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double filt_score(case_obj_t obj, long type)
{
  init();
  return score(obj, type);
}

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < CASE_TYPE; type++) {
      case_obj_clear(&one[type]);
      case_obj_clear(&zero[type]);
    }
    once = case_bool_true;
  }
}

void mutate(case_obj_t *obj1, case_obj_t *obj2)
{
  case_obj_t *o1;
  case_obj_t *o2;
  long bit;
  case_bit_t val;
  long act;
  for (act = 0; act < 4; act++) {
    bit = random() % CASE_OBJ;
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
  long smash;
  smash = case_obj_smash(obj, 0);
  for (bit = 1; bit <= smash; bit++) {
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
