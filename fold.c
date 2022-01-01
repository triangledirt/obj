#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "case.h"
#include "fold.h"
#include "obj.h"
#include "tool.h"
#include "toss.h"

#define POP 32
#define FOLDS 8

typedef case_obj_t pop_t[POP];

static double fitness[32];
static case_obj_t fittest[32];
static double fits[32][POP];
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);
static void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type);
static double getfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);
static void init(case_obj_t pop[], long type);
static void initonce();

case_bit_t fold_classify(case_obj_t obj, long type)
{
  return case_obj_comparet(obj, ideal[type]) > (0.9 * fitness[type]);
}

void fold_learn(case_obj_t obj[], long objsz, long type)
{
  pop_t pop;
  long start;
  long idx;
  long dir;
  long o;
  case_bit_t val;
  long len;
  long each;
  initonce();
  init(pop, type);
  for (each = 0; each <= FOLDS; each++) {
    o = random() % POP;
    start = random() % 32;
    val = random() % 2;
    len = random() % 3;
    dir = toss_coin() ? 1 : -1;
    for (idx = start; labs(start - idx) <= len; idx += dir)
      case_obj_setattr(&pop[o], tool_wrapidx(idx, 32), val);
    fits[type][o] = -1;
  }
  forcecalc(pop, obj, objsz, type);
  ideal[type] = fittest[type];
#if CASE_VERBOSE
  printf("type%ld ideal fld ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness[type]);
#endif
}

void calcfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type)
{
  long idx;
  double fit;
  double tot = 0;
  case_obj_t calcobj;
  calcobj = pop[o];
  for (idx = 0; idx < objsz; idx++)
    if (toss_coin())
      tot += case_obj_comparet(calcobj, obj[idx]);
  fit = tot / (objsz / 2);
  fits[type][o] = fit;
  if (fit > fitness[type]) {
    fittest[type] = calcobj;
    fitness[type] = fit;
  }
}

void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type)
{
  long o;
  for (o = 0; o < objsz; o++)
    if (fits[o] < 0)
      calcfit(pop, o, obj, objsz, type);
}

double getfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type)
{
  if (fits[o] < 0)
    calcfit(pop, o, obj, objsz, type);
  return fits[type][o];
}

void init(case_obj_t pop[], long type)
{
  long idx;
  case_obj_t obj;
  for (idx = 0; idx < POP; idx++) {
    obj = ideal[type];
    case_obj_mutate(&obj);
    pop[idx] = obj;
    fits[type][idx] = -1;
  }
  case_obj_randomize(&fittest[type]);
  for (idx = 0; idx < 32; idx++)
    fitness[type] = 0.0;
}

void initonce()
{
  long idx;
  if (!once) {
    for (idx = 0; idx < 32; idx++)
      case_obj_randomize(&ideal[idx]);
    once = 1;
  }
}
