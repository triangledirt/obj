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

static double fitness[CASE_OBJ];
static case_obj_t fittest[CASE_OBJ];
static double fits[CASE_OBJ][POP];
static case_obj_t ideal[CASE_OBJ];
static case_bool_t once = case_bool_false;

static void calcfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);
static void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type);
static double getfit(pop_t pop, long o, case_obj_t obj[], long objsz, long type);
static case_obj_t getparent(pop_t pop, case_obj_t obj[], long objsz, long type);
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
  init();
  reset(pop, type);
  for (mating = 0; mating < MATINGS; mating++) {
    parent1 = getparent(pop, obj, objsz, type);
    parent2 = getparent(pop, obj, objsz, type);
    crossover = random() % CASE_OBJ;
    for (bit = 0; bit < crossover; bit++) {
      val = case_obj_getattr(parent1, bit);
      case_obj_setattr(&child, bit, val);
    }
    for (bit = crossover; bit < CASE_OBJ; bit++) {
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
  printf("type%02ld ideal gen ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double gene_score(case_obj_t obj, long type)
{
  init();
  return case_obj_comparetypes(obj, ideal[type]);
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
