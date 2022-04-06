#include <math.h>
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

static double fitness[CASE_OBJ];
static case_obj_t fittest[CASE_OBJ];
static double fits[CASE_OBJ][POP];
static case_obj_t ideal[CASE_OBJ];
static case_bool_t once = case_bool_false;

static void calcfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);
static void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type);
static double getfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);
static void init();
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
      tot += case_obj_comparefocus(calcobj, obj[i]);
  fit = tot / (objsz / 2);
  fit = pow(fit, 2);
  fits[type][o] = fit;
  if (fit > fitness[type]) {
    fittest[type] = calcobj;
    fitness[type] = fit;
  }
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
  init();
  reset(pop, type);
  for (each = 0; each <= FOLDS; each++) {
    o = random() % POP;
    start = random() % CASE_OBJ;
    val = random() % 2;
    len = random() % 3;
    dir = coin_toss() ? 1 : -1;
    for (i = start; labs(start - i) <= len; i += dir)
      case_obj_setattr(&pop[o], indx_wrap(i, CASE_OBJ), val);
    fits[type][o] = -1;
  }
  forcecalc(pop, obj, objsz, type);
  ideal[type] = fittest[type];
#if CASE_VERBOSE
  printf("type%02ld ideal fld ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double fold_score(case_obj_t obj, long type)
{
  init();
  return case_obj_comparefocus(obj, ideal[type]);
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

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < CASE_OBJ; type++)
      case_obj_randomize(&ideal[type]);
    once = case_bool_true;
  }
}

void reset(case_obj_t pop[], long type)
{
  long i;
  case_obj_t obj;
  for (i = 0; i < POP; i++) {
    obj = ideal[type];
    case_obj_mutate(&obj);
    pop[i] = obj;
    fits[type][i] = -1;
  }
  fitness[type] = 0.0;
  case_obj_randomize(&fittest[type]);
}
