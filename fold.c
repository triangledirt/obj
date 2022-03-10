#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "fold.h"
#include "indx.h"
#include "obj.h"

#define POP 128
#define FOLDS 1

typedef case_obj_t pop_t[POP];

static double fitness[32];
static case_obj_t fittest[32];
static double fits[32][POP];
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);

static void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type);

static double getfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);

static void initonce();
static void reset(case_obj_t pop[], long type);

void calcfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type)
{
  long i;
  double fit;
  double tot = 0;
  case_obj_t calcobj;
  calcobj = pop[o];
  for (i = 0; i < objsz; i++)
    if (coin_toss())
      tot += case_obj_comparetypes(calcobj, obj[i]);
  fit = tot / (objsz / 2);
  fits[type][o] = fit;
  if (fit > fitness[type]) {
    fittest[type] = calcobj;
    fitness[type] = fit;
  }
}

case_bit_t fold_classify(case_obj_t obj, long type)
{
  initonce();
  return case_obj_comparetypes(obj, ideal[type]) > (0.9 * fitness[type]);
}

double fold_classifydouble(case_obj_t obj, long type)
{
  initonce();
  return case_obj_comparetypes(obj, ideal[type]);
}

void fold_learn(case_obj_t obj[], long objsz, long type)
{
  pop_t pop;
  long start;
  long i;
  long dir;
  long o;
  case_bit_t val;
  long len;
  long each;
  initonce();
  reset(pop, type);
  for (each = 0; each <= FOLDS; each++) {
    o = random() % POP;
    start = random() % 32;
    val = random() % 2;
    len = random() % 3;
    dir = coin_toss() ? 1 : -1;
    for (i = start; labs(start - i) <= len; i += dir)
      case_obj_setattr(&pop[o], indx_wrap(i, 32), val);
    fits[type][o] = -1;
  }
  forcecalc(pop, obj, objsz, type);
  ideal[type] = fittest[type];
#if CASE_VERBOSE
  printf("type%ld ideal fld ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type)
{
  long o;
  for (o = 0; o < objsz; o++)
    if (fits[type][o] < 0)
      calcfit(pop, o, obj, objsz, type);
}

double getfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type)
{
  if (fits[o] < 0)
    calcfit(pop, o, obj, objsz, type);
  return fits[type][o];
}

void initonce()
{
  long type;
  if (!once) {
    for (type = 0; type < 32; type++)
      case_obj_randomize(&ideal[type]);
    once = 1;
  }
}

void reset(case_obj_t pop[], long type)
{
  long i;
  case_obj_t obj;
  obj = ideal[type];
  for (i = 0; i < POP; i++) {
    case_obj_mutate(&obj);
    pop[i] = obj;
    fits[type][i] = -1;
  }
  fitness[type] = 0.0;
  case_obj_randomize(&fittest[type]);
}
