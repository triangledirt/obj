#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "case.h"
#include "gene.h"
#include "obj.h"
#include "toss.h"

#define MATINGS 16
#define POP 32

typedef case_obj_t pop_t[POP];

static double fitness = 0.0;
static case_obj_t fittest;
static double fits[POP];
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, long obj, case_obj_t objs[], long objssz);
static void forcecalc(pop_t pop, case_obj_t objs[], long objssz);
static double getfit(pop_t pop, long obj, case_obj_t objs[], long objssz);
static case_obj_t getparent(pop_t pop, case_obj_t objs[], long objssz);
static void init(case_obj_t pop[], long type);
static void initonce();

case_bit_t gene_classify(case_obj_t obj, long type)
{
  return case_obj_comparet(obj, ideal[type]) > (0.9 * fitness);
}

void gene_learn(case_obj_t objs[], long objssz, long type)
{
  long mating;
  pop_t pop;
  case_obj_t parent1;
  case_obj_t parent2;
  case_obj_t child;
  long bit;
  long crossover;
  long idx;
  case_bit_t val;
  initonce();
  init(pop, type);
  for (mating = 0; mating < MATINGS; mating++) {
    parent1 = getparent(pop, objs, objssz);
    parent2 = getparent(pop, objs, objssz);
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
    idx = random() % POP;
    pop[idx] = child;
    fits[idx] = -1;
  }
  forcecalc(pop, objs, objssz);
  ideal[type] = fittest;
#if CASE_VERBOSE
  printf("type%ld ideal gen ", type);
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
    tot += case_obj_comparet(calcobj, objs[idx]);
  fit = tot / objssz;
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

case_obj_t getparent(case_obj_t pop[], case_obj_t objs[], long objssz)
{
  long tries;
  double fit = 0;
  double newfit;
  long idx = 0;
  long newid;
  for (tries = 0; tries < 6; tries++) {
    newid = random() % POP;
    newfit = getfit(pop, newid, objs, objssz);
    if (newfit > fit) {
      idx = newid;
      fit = newfit;
    }
  }
  return pop[idx];
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
