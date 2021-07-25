#include <stdio.h>
#include <stdlib.h>
#include "abit.h"
#include "afold.h"
#include "alib.h"
#include "aobj.h"
#include "atool.h"
#include "atoss.h"

#define POP 32
#define FOLDS 8

typedef aobj_t pop_t[POP];

static double fitness = 0.0;
static aobj_t fittest;
static double fits[POP];
static aobj_t ideal[32];
static abit_t once = 0;

static void calcfit(pop_t pop, long obj, aobj_t objs[], long objs_size);
static void forcecalc(pop_t pop, aobj_t objs[], long objs_size);
static double getfit(pop_t pop, long obj, aobj_t objs[], long objs_size);
static void init(aobj_t pop[], long type);
static void initonce();

abit_t afold_classify(aobj_t obj, long type)
{
  return aobj_compareq(obj, ideal[type]) < fitness;
}

void afold_learn(aobj_t objs[], long objs_size, long type)
{
  pop_t pop;
  long start;
  long idx;
  long dir;
  long obj;
  abit_t val;
  long len;
  long each;
  initonce();
  init(pop, type);
  for (each = 0; each <= FOLDS; each++) {
    obj = random() % POP;
    start = random() % 32;
    val = random() % 2;
    len = random() % 3;
    if (atoss_coin()) {
      dir = 1;
    } else {
      dir = -1;
    }
    for (idx = start; labs(start - idx) <= len; idx += dir) {
      aobj_setattr(&pop[obj], atool_wrapidx(idx, 32), val);
    }
    fits[obj] = -1;
  }
  forcecalc(pop, objs, objs_size);
  ideal[type] = fittest;
#if ALIB_VERBOSE
  printf("type%ld ideal fld ", type);
  aobj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}

void calcfit(pop_t pop, long obj, aobj_t objs[], long objs_size)
{
  long idx;
  double fit;
  double tot = 0;
  aobj_t calcobj;
  calcobj = pop[obj];
  for (idx = 0; idx < objs_size; idx++) {
    tot += aobj_compareq(calcobj, objs[idx]);
  }
  fit = tot / objs_size;
  fits[obj] = fit;
  if (fit > fitness) {
    fittest = calcobj;
    fitness = fit;
  }
}

void forcecalc(pop_t pop, aobj_t objs[], long objs_size)
{
  long obj;
  for (obj = 0; obj < objs_size; obj++) {
    if (fits[obj] < 0) {
      calcfit(pop, obj, objs, objs_size);
    }
  }
}

double getfit(pop_t pop, long obj, aobj_t objs[], long objs_size)
{
  if (fits[obj] < 0) {
    calcfit(pop, obj, objs, objs_size);
  }
  return fits[obj];
}

void init(aobj_t pop[], long type)
{
  long idx;
  aobj_t obj;
  for (idx = 0; idx < POP; idx++) {
    obj = ideal[type];
    aobj_mutate(&obj);
    pop[idx] = obj;
    fits[idx] = -1;
  }
  aobj_randomize(&fittest);
  fitness = 0.0;
}

void initonce()
{
  long idx;
  if (!once) {
    for (idx = 0; idx < 32; idx++) {
      aobj_randomize(&ideal[idx]);
    }
    once = 1;
  }
}
