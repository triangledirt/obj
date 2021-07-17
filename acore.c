#include <stdio.h>
#include <stdlib.h>
#include "abit.h"
#include "acoord.h"
#include "acore.h"
#include "alib.h"
#include "aobj.h"
#include "atool.h"
#include "atoss.h"

#define ACTS 32
#define ANARCHY 4
#define DIM 4

typedef aobj_t pop_t[DIM][DIM][DIM];

static double fitness = 0.0;
static double fits[DIM][DIM][DIM];
static aobj_t fittest;
static aobj_t ideal[32];
static abit_t once = 0;

static void calcfit(pop_t pop, acoord_t *c, aobj_t objs[], long objs_size);
static void forcecalc(pop_t pop, aobj_t objs[], long objs_size);
static void dance(pop_t pop_t, acoord_t *dest, acoord_t *src1, acoord_t *src2);
static void findbest(pop_t pop, acoord_t *actor, acoord_t *best, aobj_t objs[],
  long objs_size);
static void findworst(pop_t pop, acoord_t *actor, acoord_t *worst,
  aobj_t objs[], long objs_size);
static double getfit(pop_t pop, acoord_t *c, aobj_t objs[], long objs_size);
static void init(pop_t pop, long type);
static void initonce();
static void randcoord(acoord_t *c);

abit_t acore_classify(aobj_t obj, long type)
{
  return aobj_compareq(obj, ideal[type]) < fitness;
}

void acore_learn(aobj_t objs[], long objs_size, long type)
{
  long act;
  acoord_t actor;
  acoord_t best;
  acoord_t worst;
  aobj_t obj;
  pop_t pop;
  initonce();
  init(pop, type);
  for (act = 0; act < ACTS; act++) {
    randcoord(&actor);
    findbest(pop, &actor, &best, objs, objs_size);
    findworst(pop, &actor, &worst, objs, objs_size);
    if (atoss_die(ANARCHY)) {
      dance(pop, &actor, &best, &worst);
    } else {
      dance(pop, &worst, &actor, &best);
    }
  }
  forcecalc(pop, objs, objs_size);
  ideal[type] = fittest;
#if ALIB_VERBOSE
  printf("type%ld ideal cor ", type);
  aobj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}

void calcfit(pop_t pop, acoord_t *c, aobj_t objs[], long objs_size)
{
  double fit;
  double tot = 0;
  long idx;
  aobj_t obj;
  obj = pop[c->x][c->y][c->z];
  for (idx = 0; idx < objs_size; idx++) {
    tot += aobj_compareq(obj, objs[idx]);
  }
  fit = tot / objs_size;
  fits[c->x][c->y][c->z] = fit;
  if (fit > fitness) {
    fittest = obj;
    fitness = fit;
  }
}

void forcecalc(pop_t pop, aobj_t objs[], long objs_size)
{
  acoord_t c;
  for (c.x = 0; c.x < DIM; c.x++) {
    for (c.y = 0; c.y < DIM; c.y++) {
      for (c.z = 0; c.z < DIM; c.z++) {
        if (fits[c.x][c.y][c.z] < 0) {
          calcfit(pop, &c, objs, objs_size);
        }
      }
    }
  }
}

void dance(pop_t pop, acoord_t *dest, acoord_t *src1, acoord_t *src2)
{
  aobj_t parent1 = pop[src1->x][src1->y][src1->z];
  aobj_t parent2 = pop[src2->x][src2->y][src2->z];
  aobj_t *child = &pop[dest->x][dest->y][dest->z];
  long bit;
  long crossover = random() % 32;
  for (bit = 0; bit < 32; bit++) {
    if (bit >= crossover) {
      aobj_setattr(child, bit, aobj_getattr(parent1, bit));
    } else {
      aobj_setattr(child, bit, aobj_getattr(parent2, bit));
    }
    aobj_mutate(child);
  }
  fits[dest->x][dest->y][dest->z] = -1;
}

void findbest(pop_t pop, acoord_t *actor, acoord_t *best, aobj_t objs[],
  long objs_size)
{
  acoord_t t;
  acoord_t c;
  double fit = 0.0;
  double f;
  for (t.x = -1; t.x < 2; t.x++) {
    for (t.y = -1; t.y < 2; t.y++) {
      for (t.z = -1; t.z < 2; t.z++) {
        c.x = atool_wrapidx(actor->x + t.x, DIM);
        c.y = atool_wrapidx(actor->y + t.y, DIM);
        c.z = atool_wrapidx(actor->z + t.z, DIM);
        if ((actor->x == c.x) && (actor->y == c.y) && (actor->z == c.z)) {
          continue;
        }
        f = getfit(pop, &c, objs, objs_size);
        if (f > fit) {
          fit = f;
          *best = c;
        }
      }
    }
  }
}

void findworst(pop_t pop, acoord_t *actor, acoord_t *worst, aobj_t objs[],
  long objs_size)
{
  acoord_t t;
  acoord_t c;
  double fit = 1.0;
  double f;
  for (t.x = -1; t.x < 2; t.x++) {
    for (t.y = -1; t.y < 2; t.y++) {
      for (t.z = -1; t.z < 2; t.z++) {
        c.x = atool_wrapidx(actor->x + t.x, DIM);
        c.y = atool_wrapidx(actor->y + t.y, DIM);
        c.z = atool_wrapidx(actor->z + t.z, DIM);
        f = getfit(pop, &c, objs, objs_size);
        if (f < fit) {
          fit = f;
          *worst = c;
        }
      }
    }
  }
}

double getfit(pop_t pop, acoord_t *c, aobj_t objs[], long objs_size)
{
  if (fits[c->x][c->y][c->z] < 0) {
    calcfit(pop, c, objs, objs_size);
  }
  return fits[c->x][c->y][c->z];
}

void init(pop_t pop, long type)
{
  struct acoord_t c;
  for (c.x = 0; c.x < DIM; c.x++) {
    for (c.y = 0; c.y < DIM; c.y++) {
      for (c.z = 0; c.z < DIM; c.z++) {
        pop[c.x][c.y][c.z] = ideal[type];
        aobj_mutate(&pop[c.x][c.y][c.z]);
        fits[c.x][c.y][c.z] = -1;
      }
    }
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

void randcoord(acoord_t *c)
{
  c->x = random() % DIM;
  c->y = random() % DIM;
  c->z = random() % DIM;
}
