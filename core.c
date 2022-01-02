#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "case.h"
#include "coord.h"
#include "core.h"
#include "index.h"
#include "obj.h"
#include "toss.h"

#define ACTS 32
#define ANARCHY 4
#define DIM 4

typedef case_obj_t pop_t[DIM][DIM][DIM];

static double fitness[32];
static double fits[32][DIM][DIM][DIM];
static case_obj_t fittest[32];
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, coord_t *c, case_obj_t obj[], long objsz, long type);

static void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type);

static void dance(pop_t pop_t, coord_t *dest, coord_t *src1, coord_t *src2, long type);

static void findbest(pop_t pop, coord_t *actor, coord_t *best, case_obj_t obj[], long objsz, long type);

static void findworst(pop_t pop, coord_t *actor, coord_t *worst, case_obj_t obj[], long objsz, long type);

static double getfit(pop_t pop, coord_t *c, case_obj_t obj[], long objsz, long type);

static void init(pop_t pop, long type);
static void initonce();
static void randcoord(coord_t *c);

case_bit_t core_classify(case_obj_t obj, long type)
{
  return case_obj_comparet(obj, ideal[type]) > (0.9 * fitness[type]);
}

void core_learn(case_obj_t obj[], long objsz, long type)
{
  long act;
  coord_t actor;
  coord_t best;
  coord_t worst;
  pop_t pop;
  initonce();
  init(pop, type);
  for (act = 0; act < ACTS; act++) {
    randcoord(&actor);
    findbest(pop, &actor, &best, obj, objsz, type);
    findworst(pop, &actor, &worst, obj, objsz, type);
    if (toss_die(ANARCHY)) {
      dance(pop, &actor, &best, &worst, type);
    } else {
      dance(pop, &worst, &actor, &best, type);
    }
  }
  forcecalc(pop, obj, objsz, type);
  ideal[type] = fittest[type];
#if CASE_VERBOSE
  printf("type%ld ideal cor ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness[type]);
#endif
}

void calcfit(pop_t pop, coord_t *c, case_obj_t obj[], long objsz, long type)
{
  double fit;
  double tot = 0;
  long i;
  case_obj_t o;
  o = pop[c->x][c->y][c->z];
  for (i = 0; i < objsz; i++)
    if (toss_coin())
      tot += case_obj_comparet(o, obj[i]);
  fit = tot / (objsz / 2);
  fits[type][c->x][c->y][c->z] = fit;
  if (fit > fitness[type]) {
    fittest[type] = o;
    fitness[type] = fit;
  }
}

void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type)
{
  coord_t c;
  for (c.x = 0; c.x < DIM; c.x++)
    for (c.y = 0; c.y < DIM; c.y++)
      for (c.z = 0; c.z < DIM; c.z++)
        if (fits[c.x][c.y][c.z] < 0)
          calcfit(pop, &c, obj, objsz, type);
}

void dance(pop_t pop, coord_t *dest, coord_t *src1, coord_t *src2, long type)
{
  case_obj_t parent1;
  case_obj_t parent2;
  case_obj_t *child;
  parent1 = pop[src1->x][src1->y][src1->z];
  parent2 = pop[src2->x][src2->y][src2->z];
  child = &pop[dest->x][dest->y][dest->z];
  long bit;
  long crossover = random() % 32;
  for (bit = 0; bit < 32; bit++) {
    if (bit >= crossover) {
      case_obj_setattr(child, bit, case_obj_getattr(parent1, bit));
    } else {
      case_obj_setattr(child, bit, case_obj_getattr(parent2, bit));
    }
    case_obj_mutate(child);
  }
  fits[type][dest->x][dest->y][dest->z] = -1;
}

void findbest(pop_t pop, coord_t *actor, coord_t *best, case_obj_t obj[], long objsz, long type)
{
  coord_t t;
  coord_t c;
  double fit = 0.0;
  double f;
  for (t.x = -1; t.x < 2; t.x++)
    for (t.y = -1; t.y < 2; t.y++)
      for (t.z = -1; t.z < 2; t.z++) {
        c.x = index_wrap(actor->x + t.x, DIM);
        c.y = index_wrap(actor->y + t.y, DIM);
        c.z = index_wrap(actor->z + t.z, DIM);
        if ((actor->x == c.x) && (actor->y == c.y) && (actor->z == c.z))
          continue;
        f = getfit(pop, &c, obj, objsz, type);
        if (f > fit) {
          fit = f;
          *best = c;
        }
      }
}

void findworst(pop_t pop, coord_t *actor, coord_t *worst, case_obj_t obj[], long objsz, long type)
{
  coord_t t;
  coord_t c;
  double fit = 1.0;
  double f;
  for (t.x = -1; t.x < 2; t.x++)
    for (t.y = -1; t.y < 2; t.y++)
      for (t.z = -1; t.z < 2; t.z++) {
        c.x = index_wrap(actor->x + t.x, DIM);
        c.y = index_wrap(actor->y + t.y, DIM);
        c.z = index_wrap(actor->z + t.z, DIM);
        f = getfit(pop, &c, obj, objsz, type);
        if (f < fit) {
          fit = f;
          *worst = c;
        }
      }
}

double getfit(pop_t pop, coord_t *c, case_obj_t obj[], long objsz, long type)
{
  if (fits[c->x][c->y][c->z] < 0)
    calcfit(pop, c, obj, objsz, type);
  return fits[type][c->x][c->y][c->z];
}

void init(pop_t pop, long type)
{
  struct coord_t c;
  long i;
  for (c.x = 0; c.x < DIM; c.x++)
    for (c.y = 0; c.y < DIM; c.y++)
      for (c.z = 0; c.z < DIM; c.z++) {
        pop[c.x][c.y][c.z] = ideal[type];
        case_obj_mutate(&pop[c.x][c.y][c.z]);
        fits[type][c.x][c.y][c.z] = -1;
      }
  case_obj_randomize(&fittest[type]);
  for (i = 0; i < 32; i++)
    fitness[i] = 0.0;
}

void initonce()
{
  long i;
  if (!once) {
    for (i = 0; i < 32; i++)
      case_obj_randomize(&ideal[i]);
    once = 1;
  }
}

void randcoord(coord_t *c)
{
  c->x = random() % DIM;
  c->y = random() % DIM;
  c->z = random() % DIM;
}