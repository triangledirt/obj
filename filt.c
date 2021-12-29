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

static void calcfit(case_obj_t obj[], long objsz);
static void initonce();
static void restore();
static void save();
static double score(case_object_t obj, long type);

void calcfit(case_obj_t obj[], long objsz)
{
  long idx;
  case_obj_t o;
  double score = 0.0;
  for (idx = 0; idx < objsz; idx++) {
    o = obj[idx];
    score += score(o);
  }
  fitness = score / objsz;
}

case_bit_t filt_classify(case_obj_t obj, long type)
{
  return score(obj, type) > 0.9;
}

void filt_learn(case_obj_t obj[], long objsz, long type)
{
  long idx;
  long bit;
  case_obj_t o;
  long act;
#if CASE_VERBOSE
  double tot = 0;
#endif
  initonce();
  for (act = 0; act < ACTS; act++) {
    calcfitness(obj, objsz);
    save();
    case_object_mutate(&one);
    case_object_mutate(&zero);
    calcfitness(obj, objsz);
    if (fitnesssv[type] > fitness[type])
      restore();
  }
#if CASE_VERBOSE
  for (idx = 0; idx < objsz; idx++)
    tot += case_obj_comparet(ideal[type], obj[idx]);
  fitness[type] = tot / (objsz / 2);
  printf("type%ld ideal filt ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness[type]);
#endif
}

void initonce()
{
  if (!once) {
    case_obj_randomize(&one);
    case_obj_randomize(&zero);
    once = 1;
  }
}

void restore()
{
  one = onesv;
  zero = zerosv;
  fitness = fitnesssv;
}

void save()
{
  onesv = one;
  zerosv = zero;
  fitnesssv = fitness;
}

double score(case_obj_t obj, long type)
{
  long bit;
  long onetot = 0;
  long zerotot = 0;
  long onematch = 0;
  long zeromatch = 0;
  for (bit = 1; bit < 32; bit++) {
    onetot += case_obj_getattr(one, bit);
    zerotot += case_obj_getattr(zero, bit);
  }
  for (bit = 1; bit < 32; bit++) {
    if (case_obj_getattr(one[type], bit) && case_obj_getattr(obj, bit))
      onematch++;
    if (case_obj_getattr(zero[type], bit) && !case_obj_getattr(obj, bit))
      zeromatch++;
  }
  return (onematch + zeromatch) / (onetot + zerotot);
}
