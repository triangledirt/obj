#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "class.h"
#include "coin.h"
#include "gene.h"
#include "obj.h"

#define MATINGS 16
#define POP 32

typedef obj_t pop_t[POP];

static double fitness[OBJ_TYPE];
static obj_t fittest[OBJ_TYPE];
static double fits[OBJ_TYPE][POP];
static obj_t ideal[OBJ_TYPE];
static enum obj_bool_t once = obj_bool_false;

static void calcfit(pop_t pop, long o, obj_t obj[], long objsz, long type);
static void forcecalc(pop_t pop, obj_t obj[], long objsz, long type);
static double getfit(pop_t pop, long o, obj_t obj[], long objsz, long type);
static obj_t getparent(pop_t pop, obj_t obj[], long objsz, long type);
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

double obj_gene_fit(long type)
{
  return fitness[type];
}

void forcecalc(pop_t pop, obj_t obj[], long objsz, long type)
{
  long o;
  for (o = 0; o < objsz; o++)
    if (fits[type][o] < 0)
      calcfit(pop, o, obj, objsz, type);
}

void obj_gene_learn(obj_t obj[], long objsz, long type)
{
  long mating;
  pop_t pop;
  obj_t parent1;
  obj_t parent2;
  obj_t child;
  long bit;
  long crossover;
  long i;
  obj_bit_t val;
  init();
  reset(pop, type);
  for (mating = 0; mating < MATINGS; mating++) {
    parent1 = getparent(pop, obj, objsz, type);
    parent2 = getparent(pop, obj, objsz, type);
    crossover = random() % OBJ;
    for (bit = 0; bit < crossover; bit++) {
      val = obj_attr(parent1, bit);
      obj_setattr(&child, bit, val);
    }
    for (bit = crossover; bit < OBJ; bit++) {
      val = obj_attr(parent2, bit);
      obj_setattr(&child, bit, val);
    }
    obj_mutate(&child);
    i = random() % POP;
    pop[i] = child;
    fits[type][i] = -1;
  }
  forcecalc(pop, obj, objsz, type);
  ideal[type] = fittest[type];
#if OBJ_VERBOSE
  printf("type%02ld ideal gen ", type);
  obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double obj_gene_score(obj_t obj, long type)
{
  init();
  return obj_compare(obj, ideal[type]);
}

double getfit(pop_t pop, long o, obj_t obj[], long objsz, long type)
{
  if (fits[type][o] < 0)
    calcfit(pop, o, obj, objsz, type);
  return fits[type][o];
}

obj_t getparent(obj_t pop[], obj_t obj[], long objsz, long type)
{
  long tries;
  double fit = 0;
  double newfit;
  long i = 0;
  long newid;
  for (tries = 0; tries < 6; tries++) {
    newid = random() % POP;
    newfit = getfit(pop, newid, obj, objsz, type);
    if (newfit > fit) {
      i = newid;
      fit = newfit;
    }
  }
  return pop[i];
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
