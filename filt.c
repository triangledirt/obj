#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "filt.h"
#include "obj.h"
#include "toss.h"

#define ACTS 8

static case_bit_t once = 0;
static case_obj_t one[32];
static case_obj_t onesv[32];
static double fitness[32];
static double fitnesssv[32];
static case_obj_t zero[32];
static case_obj_t zerosv[32];

static void calcfit(case_obj_t obj[], long objsz, long type);
static void initonce();
static void restore(long type);
static void save(long type);
static double score(case_obj_t obj, long type);

void calcfit(case_obj_t obj[], long objsz, long type)
{
  long idx;
  case_obj_t o;
  double s = 0.0;
  for (idx = 0; idx < objsz; idx++) {
    o = obj[idx];
    s += score(o, type);
  }
  fitness[type] = s / objsz;
}

case_bit_t filt_classify(case_obj_t obj, long type)
{
  return score(obj, type) > (0.9 * fitness[type]);
}

void filt_learn(case_obj_t obj[], long objsz, long type)
{
  long idx;
  long bit;
  case_obj_t o;
  long act;
  initonce();
  for (act = 0; act < ACTS; act++) {
    calcfit(obj, objsz, type);
    save(type);
    case_obj_mutate(&one[type]);
    case_obj_mutate(&zero[type]);
    calcfit(obj, objsz, type);
    if (fitnesssv[type] > fitness[type])
      restore(type);
  }
#if CASE_VERBOSE
  printf("            flt                                  %0.3f%%\n", fitness[type]);
#endif
}

void initonce()
{
  long type;
  if (!once) {
    for (type = 0; type < 32; type++) {
      case_obj_randomize(&one[type]);
      case_obj_randomize(&zero[type]);
    }
    once = 1;
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
  for (bit = 1; bit < 32; bit++) {
    onetot += case_obj_getattr(one[type], bit);
    zerotot += case_obj_getattr(zero[type], bit);
  }
  for (bit = 1; bit < 32; bit++) {
    if (case_obj_getattr(one[type], bit) && case_obj_getattr(obj, bit))
      onematch++;
    if (case_obj_getattr(zero[type], bit) && !case_obj_getattr(obj, bit))
      zeromatch++;
  }
  return (onematch + zeromatch) / 1 + (onetot + zerotot);
}
