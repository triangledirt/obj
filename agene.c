#include <stdio.h>
#include <stdlib.h>
#include "abit.h"
#include "agene.h"
#include "alib.h"
#include "aobj.h"
#include "atoss.h"

#define MATINGS 16
#define POP 32

typedef aobj_t pop_t[POP];

static double fitness;
static aobj_t fittest;
static double fits[POP];
static aobj_t ideal[32];
static abit_t once = 0;

static void calcfit(pop_t pop, long obj, aobj_t objs[], long objs_size);
static double getfit(pop_t pop, long obj, aobj_t objs[], long objs_size);
static aobj_t getparent(pop_t pop, aobj_t objs[], long objs_size);
static void init(aobj_t pop[], long type);
static void initonce();

abit_t agene_classify(aobj_t obj, long type)
{
  abit_t class;
  if (aobj_compare(obj, ideal[type]) > 0.75) {
    class = 1;
  } else {
    class = 0;
  }
  return class;
}

void agene_learn(aobj_t objs[], long objs_size, long type)
{
  long mating;
  pop_t pop;
  aobj_t parent1;
  aobj_t parent2;
  aobj_t child;
  long bit;
  long crossover;
  long idx;
  abit_t val;
#if ALIB_VERBOSE
  double tot = 0;
#endif
  initonce();
  init(pop, type);
  for (mating = 0; mating < MATINGS; mating++) {
    parent1 = getparent(pop, objs, objs_size);
    parent2 = getparent(pop, objs, objs_size);
    crossover = random() % 32;
    for (bit = 0; bit < 32; bit++) {
      if (bit >= crossover) {
        val = aobj_getattr(parent2, bit);
      } else {
        val = aobj_getattr(parent1, bit);
      }
      aobj_setattr(&child, bit, val);
      aobj_mutate(&child);
    }
    idx = random() % POP;
    pop[idx] = child;
    fits[idx] = -1;
  }
  ideal[type] = fittest;
#if ALIB_VERBOSE
  printf("type%ld ideal gen ", type);
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
    tot += aobj_compare(calcobj, objs[idx]);
  }
  fit = tot / objs_size;
  fits[obj] = fit;
  if (fit > fitness) {
    fittest = calcobj;
    fitness = fit;
  }
}

double getfit(pop_t pop, long obj, aobj_t objs[], long objs_size)
{
  if (fits[obj] < 0) {
    calcfit(pop, obj, objs, objs_size);
  }
  return fits[obj];
}

aobj_t getparent(aobj_t pop[], aobj_t objs[], long objs_size)
{
  long tries;
  double fit = 0;
  double newfit;
  long idx = 0;
  long newid;
  for (tries = 0; tries < 6; tries++) {
    newid = random() % POP;
    newfit = getfit(pop, newid, objs, objs_size);
    if (newfit > fit) {
      idx = newid;
      fit = newfit;
    }
  }
  return pop[idx];
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
