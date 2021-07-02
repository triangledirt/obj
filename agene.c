#include <stdio.h>
#include <stdlib.h>
#include "abit.h"
#include "agene.h"
#include "alib.h"
#include "aobj.h"
#include "atoss.h"

#define MATINGS 16
#define POP 32

static double fitness;
static double fits[POP];
static aobj_t ideal[ALIB_TYPE_COUNT];
static abit_t once = 0;

static void calcfit(long obj, aobj_t objs[], long objs_size);
static double getfit(long obj, aobj_t objs[], long objs_size);
static aobj_t getfittest(aobj_t pop[], aobj_t objs[], long objs_size);
static aobj_t getparent(aobj_t pop[], aobj_t objs[], long objs_size);
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
  aobj_t pop[POP];
  aobj_t parent1;
  aobj_t parent2;
  aobj_t child;
  long bit;
  long crossover;
  long idx;
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
        aobj_setattr(&child, bit, aobj_getattr(parent1, bit));
      } else {
        aobj_setattr(&child, bit, aobj_getattr(parent2, bit));
      }
      aobj_mutate(&child);
    }
    idx = random() % POP;
    pop[idx] = child;
    fits[idx] = -1;
  }
  ideal[type] = getfittest(pop, objs, objs_size);
#if ALIB_VERBOSE
  printf("type%ld ideal gen ", type);
  aobj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}

void calcfit(long obj, aobj_t objs[], long objs_size)
{
  long idx;
  double fit = 0;
  for (idx = 0; idx < objs_size; idx++) {
    fit += aobj_compare(objs[obj], objs[idx]);
  }
  fits[obj] = fit / objs_size;
}

double getfit(long obj, aobj_t objs[], long objs_size)
{
  if (fits[obj] < 0) {
    calcfit(obj, objs, objs_size);
  }
  return fits[obj];
}

aobj_t getfittest(aobj_t pop[], aobj_t objs[], long objs_size)
{
  long idx;
  long idxfittest = 0;
  double fit = 0;
  for (idx = 0; idx < POP; idx++) {
    fit = getfit(idx, objs, objs_size);
    if (fit > fitness) {
      fitness = fit;
      idxfittest = idx;
    }
  }
  return pop[idxfittest];
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
    newfit = getfit(newid, objs, objs_size);
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
  fitness = 0.0;
}

void initonce()
{
  long idx;
  if (!once) {
    for (idx = 0; idx < ALIB_TYPE_COUNT; idx++) {
      aobj_randomize(&ideal[idx]);
    }
    once = 1;
  }
}
