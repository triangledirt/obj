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

static double fitness = 0.0;
static case_obj_t fittest;
static double fits[POP];
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, long obj, case_obj_t objs[], long objssz);
static void forcecalc(pop_t pop, case_obj_t objs[], long objssz);
static double getfit(pop_t pop, long obj, case_obj_t objs[], long objssz);
static void init(case_obj_t pop[], long type);
static void initonce();

case_bit_t fold_classify(case_obj_t obj, long type)
{
  return case_obj_comparet(obj, ideal[type]) > (0.9 * fitness);
}

void fold_learn(case_obj_t objs[], long objssz, long type)
{
  pop_t pop;
  long start;
  long idx;
  long dir;
  long obj;
  case_bit_t val;
  long len;
  long each;
  initonce();
  init(pop, type);
  for (each = 0; each <= FOLDS; each++) {
    obj = random() % POP;
    start = random() % 32;
    val = random() % 2;
    len = random() % 3;
    dir = toss_coin() ? 1 : -1;
    for (idx = start; labs(start - idx) <= len; idx += dir)
      case_obj_setattr(&pop[obj], tool_wrapidx(idx, 32), val);
    fits[obj] = -1;
  }
  forcecalc(pop, objs, objssz);
  ideal[type] = fittest;
#if CASE_VERBOSE
  printf("type%ld ideal fld ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}

void calcfit(pop_t pop, long obj, case_obj_t objs[], long objssz)
{
  long idx;
  double fit;
  double tot = 0;
  case_obj_t calcobj;
  calcobj = pop[obj];
  for (idx = 0; idx < objssz; idx++)
    if (toss_coin())
      tot += case_obj_comparet(calcobj, objs[idx]);
  fit = tot / (objssz / 2);
  fits[obj] = fit;
  if (fit > fitness) {
    fittest = calcobj;
    fitness = fit;
  }
}

void forcecalc(pop_t pop, case_obj_t objs[], long objssz)
{
  long obj;
  for (obj = 0; obj < objssz; obj++)
    if (fits[obj] < 0)
      calcfit(pop, obj, objs, objssz);
}

double getfit(pop_t pop, long obj, case_obj_t objs[], long objssz)
{
  if (fits[obj] < 0)
    calcfit(pop, obj, objs, objssz);
  return fits[obj];
}

void init(case_obj_t pop[], long type)
{
  long idx;
  case_obj_t obj;
  for (idx = 0; idx < POP; idx++) {
    obj = ideal[type];
    case_obj_mutate(&obj);
    pop[idx] = obj;
    fits[idx] = -1;
  }
  case_obj_randomize(&fittest);
  fitness = 0.0;
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
