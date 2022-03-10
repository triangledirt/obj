#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "gene.h"
#include "obj.h"

#define MATINGS 16
#define POP 32

typedef case_obj_t pop_t[POP];

static double fitness[32];
static case_obj_t fittest[32];
static double fits[32][POP];
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);

static void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type);

static double getfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);

static case_obj_t getparent(pop_t pop, case_obj_t obj[], long objsz, long type);

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
  fit = pow(fit, 2);
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
    if (fits[type][o] < 0)
      calcfit(pop, o, obj, objsz, type);
}

double gene_classify(case_obj_t obj, long type)
{
  initonce();
  return case_obj_comparetypes(obj, ideal[type]);
}

void gene_learn(case_obj_t obj[], long objsz, long type)
{
  long mating;
  pop_t pop;
  case_obj_t parent1;
  case_obj_t parent2;
  case_obj_t child;
  long bit;
  long crossover;
  long i;
  case_bit_t val;
  initonce();
  reset(pop, type);
  for (mating = 0; mating < MATINGS; mating++) {
    parent1 = getparent(pop, obj, objsz, type);
    parent2 = getparent(pop, obj, objsz, type);
    crossover = random() % 32;
    for (bit = 0; bit < crossover; bit++) {
      val = case_obj_getattr(parent1, bit);
      case_obj_setattr(&child, bit, val);
    }
    for (bit = crossover; bit < 32; bit++) {
      val = case_obj_getattr(parent2, bit);
      case_obj_setattr(&child, bit, val);
    }
    case_obj_mutate(&child);
    i = random() % POP;
    pop[i] = child;
    fits[type][i] = -1;
  }
  forcecalc(pop, obj, objsz, type);
  ideal[type] = fittest[type];
#if CASE_VERBOSE
  printf("type%ld ideal gen ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double getfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type)
{
  if (fits[type][o] < 0)
    calcfit(pop, o, obj, objsz, type);
  return fits[type][o];
}

case_obj_t getparent(case_obj_t pop[], case_obj_t obj[], long objsz, long type)
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
  for (i = 0; i < POP; i++) {
    obj = ideal[type];
    case_obj_mutate(&obj);
    pop[i] = obj;
    fits[type][i] = -1;
  }
  fitness[type] = 0.0;
  case_obj_randomize(&fittest[type]);
}
