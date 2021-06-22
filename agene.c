#include <stdio.h>
#include <stdlib.h>
#include "abit.h"
#include "agene.h"
#include "alib.h"
#include "aobj.h"

#define MATINGS 64
#define MUTATE 1024
#define POP 256

static aobj_t ideal[ALIB_TYPE_COUNT];
static double fits[POP];

static void calcfit(long obj, aobj_t objs[], long objs_size);
static double getfit(long obj, aobj_t objs[], long objs_size);
static aobj_t getfittest(aobj_t pop[], aobj_t objs[], long objs_size);
static aobj_t getparent(aobj_t pop[], aobj_t objs[], long objs_size);
static void initfits();
static void initpop(aobj_t pop[], long type);

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
  initpop(pop, type);
  initfits();
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
      if (0 == (random() % MUTATE)) {
        aobj_mutate(&child);
      }
    }
    idx = random() % POP;
    pop[idx] = child;
    fits[idx] = -1;
  }
  ideal[type] = getfittest(pop, objs, objs_size);
#if ALIB_VERBOSE
  printf("type%ld ideal gen ", type);
  aobj_print(ideal[type]);
  printf("\n");
#endif
}

void calcfit(long obj, aobj_t objs[], long objs_size)
{
  long idx;
  double fit = 0;
  if (fits[obj] < 0) {
    for (idx = 0; idx < objs_size; idx++) {
      fit += aobj_compare(objs[obj], objs[idx]);
    }
    fits[obj] = fit / objs_size;
  }
}

double getfit(long obj, aobj_t objs[], long objs_size)
{
  calcfit(obj, objs, objs_size);
  return fits[obj];
}

aobj_t getfittest(aobj_t pop[], aobj_t objs[], long objs_size)
{
  long idx;
  long idxfittest = 0;
  double fitness;
  double fittest = -1;
  for (idx = 0; idx < POP; idx++) {
    fitness = getfit(idx, objs, objs_size);
    if (fitness > fittest) {
      fittest = fitness;
      idxfittest = idx;
    }
  }
  return pop[idxfittest];
}

aobj_t getparent(aobj_t pop[], aobj_t objs[], long objs_size)
{
  long tries;
  double fitness = -1;
  double newfit;
  long idx = 0;
  long newid;
  for (tries = 0; tries < (POP / 16); tries++) {
    newid = random() % POP;
    newfit = getfit(newid, objs, objs_size);
    if (newfit > fitness) {
      idx = newid;
      fitness = newfit;
    }
  }
  return pop[idx];
}

void initfits()
{
  long idx;
  for (idx = 0; idx < POP; idx++) {
    fits[idx] = -1;
  }
}

void initpop(aobj_t pop[], long type)
{
  long indiv;
  aobj_t obj;
  for (indiv = 0; indiv < POP; indiv++) {
    if (0 == (random() % 2)) {
      obj = ideal[type];
    } else {
      aobj_randomize(&obj);
    }
    pop[indiv] = obj;
  }
}
