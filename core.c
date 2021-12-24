#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "case.h"
#include "coord.h"
#include "core.h"
#include "obj.h"
#include "tool.h"
#include "toss.h"

#define ACTS 32
#define ANARCHY 4
#define DIM 4

typedef case_obj_t pop_t[DIM][DIM][DIM];

static double fitness = 0.0;
static double fits[DIM][DIM][DIM];
static case_obj_t fittest;
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, coord_t *c, case_obj_t objs[], long objs_size);
static void forcecalc(pop_t pop, case_obj_t objs[], long objs_size);
static void dance(pop_t pop_t, coord_t *dest, coord_t *src1, coord_t *src2);
static void findbest(pop_t pop, coord_t *actor, coord_t *best, case_obj_t objs[], long objs_size);
static void findworst(pop_t pop, coord_t *actor, coord_t *worst, case_obj_t objs[], long objs_size);
static double getfit(pop_t pop, coord_t *c, case_obj_t objs[], long objs_size);
static void init(pop_t pop, long type);
static void initonce();
static void randcoord(coord_t *c);

case_bit_t core_classify(case_obj_t obj, long type)
{
  return case_obj_comparet(obj, ideal[type]) > (0.9 * fitness);
}

void core_learn(case_obj_t objs[], long objs_size, long type)
{
  long act;
  coord_t actor;
  coord_t best;
  coord_t worst;
  case_obj_t obj;
  pop_t pop;
  initonce();
  init(pop, type);
  for (act = 0; act < ACTS; act++) {
    randcoord(&actor);
    findbest(pop, &actor, &best, objs, objs_size);
    findworst(pop, &actor, &worst, objs, objs_size);
    if (toss_die(ANARCHY)) {
      dance(pop, &actor, &best, &worst);
    } else {
      dance(pop, &worst, &actor, &best);
    }
  }
  forcecalc(pop, objs, objs_size);
  ideal[type] = fittest;
#if CASE_VERBOSE
  printf("type%ld ideal cor ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}

void calcfit(pop_t pop, coord_t *c, case_obj_t objs[], long objs_size)
{
  double fit;
  double tot = 0;
  long idx;
  case_obj_t obj;
  obj = pop[c->x][c->y][c->z];
  for (idx = 0; idx < objs_size; idx++) {
    tot += case_obj_comparet(obj, objs[idx]);
  }
  fit = tot / objs_size;
  fits[c->x][c->y][c->z] = fit;
  if (fit > fitness) {
    fittest = obj;
    fitness = fit;
  }
}

void forcecalc(pop_t pop, case_obj_t objs[], long objs_size)
{
  coord_t c;
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

void dance(pop_t pop, coord_t *dest, coord_t *src1, coord_t *src2)
{
  case_obj_t parent1 = pop[src1->x][src1->y][src1->z];
  case_obj_t parent2 = pop[src2->x][src2->y][src2->z];
  case_obj_t *child = &pop[dest->x][dest->y][dest->z];
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
  fits[dest->x][dest->y][dest->z] = -1;
}

void findbest(pop_t pop, coord_t *actor, coord_t *best, case_obj_t objs[], long objs_size)
{
  coord_t t;
  coord_t c;
  double fit = 0.0;
  double f;
  for (t.x = -1; t.x < 2; t.x++) {
    for (t.y = -1; t.y < 2; t.y++) {
      for (t.z = -1; t.z < 2; t.z++) {
        c.x = tool_wrapidx(actor->x + t.x, DIM);
        c.y = tool_wrapidx(actor->y + t.y, DIM);
        c.z = tool_wrapidx(actor->z + t.z, DIM);
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

void findworst(pop_t pop, coord_t *actor, coord_t *worst, case_obj_t objs[], long objs_size)
{
  coord_t t;
  coord_t c;
  double fit = 1.0;
  double f;
  for (t.x = -1; t.x < 2; t.x++) {
    for (t.y = -1; t.y < 2; t.y++) {
      for (t.z = -1; t.z < 2; t.z++) {
        c.x = tool_wrapidx(actor->x + t.x, DIM);
        c.y = tool_wrapidx(actor->y + t.y, DIM);
        c.z = tool_wrapidx(actor->z + t.z, DIM);
        f = getfit(pop, &c, objs, objs_size);
        if (f < fit) {
          fit = f;
          *worst = c;
        }
      }
    }
  }
}

double getfit(pop_t pop, coord_t *c, case_obj_t objs[], long objs_size)
{
  if (fits[c->x][c->y][c->z] < 0) {
    calcfit(pop, c, objs, objs_size);
  }
  return fits[c->x][c->y][c->z];
}

void init(pop_t pop, long type)
{
  struct coord_t c;
  for (c.x = 0; c.x < DIM; c.x++) {
    for (c.y = 0; c.y < DIM; c.y++) {
      for (c.z = 0; c.z < DIM; c.z++) {
        pop[c.x][c.y][c.z] = ideal[type];
        case_obj_mutate(&pop[c.x][c.y][c.z]);
        fits[c.x][c.y][c.z] = -1;
      }
    }
  }
  case_obj_randomize(&fittest);
  fitness = 0.0;
}

void initonce()
{
  long idx;
  if (!once) {
    for (idx = 0; idx < 32; idx++) {
      case_obj_randomize(&ideal[idx]);
    }
    once = 1;
  }
}

void randcoord(coord_t *c)
{
  c->x = random() % DIM;
  c->y = random() % DIM;
  c->z = random() % DIM;
}
