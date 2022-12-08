#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "class.h"
#include "coin.h"
#include "fold.h"
#include "indx.h"
#include "obj.h"

#define POP 128
#define FOLDS 1

typedef obj_t pop_t[POP];

static double fitness[OBJ_TYPE];
static obj_t fittest[OBJ_TYPE];
static double fits[OBJ_TYPE][POP];
static obj_t ideal[OBJ_TYPE];
static obj_bool_t once = obj_bool_false;

static void calcfit(pop_t pop, long o, obj_t obj[], long objsz, long type);
static void forcecalc(pop_t pop, obj_t obj[], long objsz, long type);
static double getfit(pop_t pop, long o, obj_t obj[], long objsz, long type);
static void init();
static void reset(obj_t pop[], long type);

void calcfit(pop_t pop, long o, obj_t obj[], long objsz, long type)
{
  long i;
  double fit;
  double tot = 0;
  obj_t calcobj;
  calcobj = pop[o];
  for (i = 0; i < objsz; i++)
    if (obj_coin_toss())
      tot += obj_compare(calcobj, obj[i]);
  fit = tot / (objsz / 2);
  fit = pow(fit, 2);
  fits[type][o] = fit;
  if (fit > fitness[type]) {
    fittest[type] = calcobj;
    fitness[type] = fit;
  }
}

double obj_fold_fitness(long type)
{
  return fitness[type];
}

void obj_fold_learn(obj_t obj[], long objsz, long type)
{
  pop_t pop;
  long start;
  long i;
  long dir;
  long o;
  obj_bit_t val;
  long len;
  long each;
  init();
  reset(pop, type);
  for (each = 0; each <= FOLDS; each++) {
    o = random() % POP;
    start = random() % OBJ;
    val = random() % 2;
    len = random() % 3;
    dir = obj_coin_toss() ? 1 : -1;
    for (i = start; labs(start - i) <= len; i += dir)
      obj_setattr(&pop[o], obj_indx_wrap(i, OBJ), val);
    fits[type][o] = -1;
  }
  forcecalc(pop, obj, objsz, type);
  ideal[type] = fittest[type];
#if OBJ_VERBOSE
  printf("type%02ld ideal fld ", type);
  obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double obj_fold_score(obj_t obj, long type)
{
  init();
  return obj_compare(obj, ideal[type]);
}

void forcecalc(pop_t pop, obj_t obj[], long objsz, long type)
{
  long o;
  for (o = 0; o < objsz; o++)
    if (fits[type][o] < 0)
      calcfit(pop, o, obj, objsz, type);
}

double getfit(pop_t pop, long o, obj_t obj[], long objsz, long type)
{
  if (fits[o] < 0)
    calcfit(pop, o, obj, objsz, type);
  return fits[type][o];
}

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < OBJ_TYPE; type++)
      obj_randomize(&ideal[type]);
    once = obj_bool_true;
  }
}

void reset(obj_t pop[], long type)
{
  long i;
  obj_t obj;
  for (i = 0; i < POP; i++) {
    obj = ideal[type];
    obj_mutate(&obj);
    pop[i] = obj;
    fits[type][i] = -1;
  }
  fitness[type] = 0.0;
  obj_randomize(&fittest[type]);
}
