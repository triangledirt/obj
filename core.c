#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "class.h"
#include "coin.h"
#include "coord.h"
#include "core.h"
#include "die.h"
#include "indx.h"
#include "obj.h"

#define TIMES 32
#define ANARCHY 4
#define DIM 4

typedef obj_t pop_t[DIM][DIM][DIM];

static double fitness[OBJ_CLASS_TYPE];
static double fits[OBJ_CLASS_TYPE][DIM][DIM][DIM];
static obj_t fittest[OBJ_CLASS_TYPE];
static obj_t ideal[OBJ_CLASS_TYPE];
static obj_bool_t once = obj_bool_false;

static void calcfit(pop_t pop, struct obj_coord_t *c, obj_t obj[], long objsz, long type);
static void forcecalc(pop_t pop, obj_t obj[], long objsz, long type);
static void dance(pop_t pop_t, struct obj_coord_t *dest, struct obj_coord_t *src1, struct obj_coord_t *src2, long type);
static void findbest(pop_t pop, struct obj_coord_t *actor, struct obj_coord_t *best, obj_t obj[], long objsz, long type);
static void findworst(pop_t pop, struct obj_coord_t *actor, struct obj_coord_t *worst, obj_t obj[], long objsz, long type);
static double getfit(pop_t pop, struct obj_coord_t *c, obj_t obj[], long objsz, long type);
static void init();
static void randcoord(struct obj_coord_t *c);
static void reset(pop_t pop, long type);

void calcfit(pop_t pop, struct obj_coord_t *c, obj_t obj[], long objsz, long type)
{
  double fit;
  double tot = 0;
  long i;
  obj_t o;
  o = pop[c->x][c->y][c->z];
  for (i = 0; i < objsz; i++)
    if (obj_coin_toss())
      tot += obj_compare(o, obj[i]);
  fit = tot / (objsz / 2);
  fits[type][c->x][c->y][c->z] = fit;
  if (fit > fitness[type]) {
    fittest[type] = o;
    fitness[type] = fit;
  }
}

void obj_core_learn(obj_t obj[], long objsz, long type)
{
  long time;
  struct obj_coord_t actor;
  struct obj_coord_t best;
  struct obj_coord_t worst;
  pop_t pop;
  init();
  reset(pop, type);
  for (time = 0; time < TIMES; time++) {
    randcoord(&actor);
    findbest(pop, &actor, &best, obj, objsz, type);
    findworst(pop, &actor, &worst, obj, objsz, type);
    if (obj_die_toss(ANARCHY)) {
      dance(pop, &actor, &best, &worst, type);
    } else {
      dance(pop, &worst, &actor, &best, type);
    }
  }
  forcecalc(pop, obj, objsz, type);
  ideal[type] = fittest[type];
#if OBJ_VERBOSE
  printf("type%02ld ideal cor ", type);
  obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double obj_core_score(obj_t obj, long type)
{
  init();
  return obj_compare(obj, ideal[type]);
}

void dance(pop_t pop, struct obj_coord_t *dest, struct obj_coord_t *src1, struct obj_coord_t *src2, long type)
{
  obj_t parent1;
  obj_t parent2;
  obj_t *child;
  parent1 = pop[src1->x][src1->y][src1->z];
  parent2 = pop[src2->x][src2->y][src2->z];
  child = &pop[dest->x][dest->y][dest->z];
  long bit;
  long crossover = random() % OBJ;
  for (bit = 0; bit < OBJ; bit++) {
    if (bit >= crossover) {
      obj_setattr(child, bit, obj_getattr(parent1, bit));
    } else {
      obj_setattr(child, bit, obj_getattr(parent2, bit));
    }
    obj_mutate(child);
  }
  fits[type][dest->x][dest->y][dest->z] = -1;
}

void findbest(pop_t pop, struct obj_coord_t *actor, struct obj_coord_t *best, obj_t obj[], long objsz, long type)
{
  struct obj_coord_t t;
  struct obj_coord_t c;
  double fit = 0.0;
  double f;
  obj_bool_t found = obj_bool_false;
  for (t.x = -1; t.x < 2; t.x++)
    for (t.y = -1; t.y < 2; t.y++)
      for (t.z = -1; t.z < 2; t.z++) {
        c.x = obj_indx_wrap(actor->x + t.x, DIM);
        c.y = obj_indx_wrap(actor->y + t.y, DIM);
        c.z = obj_indx_wrap(actor->z + t.z, DIM);
        if ((actor->x == c.x) && (actor->y == c.y) && (actor->z == c.z))
          continue;
        f = getfit(pop, &c, obj, objsz, type);
        if (f > fit) {
          fit = f;
          *best = c;
	  found = obj_bool_true;
	}
      }
  if (!found) {
    c.x = obj_indx_wrap(actor->x + ((random() % 3) - 1), DIM);
    c.y = obj_indx_wrap(actor->y + ((random() % 3) - 1), DIM);
    c.z = obj_indx_wrap(actor->z + ((random() % 3) - 1), DIM);
    *best = c;
  }
}

void findworst(pop_t pop, struct obj_coord_t *actor, struct obj_coord_t *worst, obj_t obj[], long objsz, long type)
{
  struct obj_coord_t t;
  struct obj_coord_t c;
  double fit = 1.0;
  double f;
  obj_bool_t found = obj_bool_false;
  for (t.x = -1; t.x < 2; t.x++)
    for (t.y = -1; t.y < 2; t.y++)
      for (t.z = -1; t.z < 2; t.z++) {
        c.x = obj_indx_wrap(actor->x + t.x, DIM);
        c.y = obj_indx_wrap(actor->y + t.y, DIM);
        c.z = obj_indx_wrap(actor->z + t.z, DIM);
        f = getfit(pop, &c, obj, objsz, type);
        if (f < fit) {
          fit = f;
          *worst = c;
	  found = obj_bool_true;
        }
      }
  if (!found) {
    c.x = obj_indx_wrap(actor->x + ((random() % 3) - 1), DIM);
    c.y = obj_indx_wrap(actor->y + ((random() % 3) - 1), DIM);
    c.z = obj_indx_wrap(actor->z + ((random() % 3) - 1), DIM);
    *worst = c;
  }
}

void forcecalc(pop_t pop, obj_t obj[], long objsz, long type)
{
  struct obj_coord_t c;
  for (c.x = 0; c.x < DIM; c.x++)
    for (c.y = 0; c.y < DIM; c.y++)
      for (c.z = 0; c.z < DIM; c.z++)
        if (fits[type][c.x][c.y][c.z] < 0)
          calcfit(pop, &c, obj, objsz, type);
}

double getfit(pop_t pop, struct obj_coord_t *c, obj_t obj[], long objsz, long type)
{
  if (fits[type][c->x][c->y][c->z] < 0)
    calcfit(pop, c, obj, objsz, type);
  return fits[type][c->x][c->y][c->z];
}

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < OBJ_CLASS_TYPE; type++)
      obj_randomize(&ideal[type]);
    once = obj_bool_true;
  }
}

void randcoord(struct obj_coord_t *c)
{
  c->x = random() % DIM;
  c->y = random() % DIM;
  c->z = random() % DIM;
}

void reset(pop_t pop, long type)
{
  struct obj_coord_t c;
  for (c.x = 0; c.x < DIM; c.x++)
    for (c.y = 0; c.y < DIM; c.y++)
      for (c.z = 0; c.z < DIM; c.z++) {
        pop[c.x][c.y][c.z] = ideal[type];
        obj_mutate(&pop[c.x][c.y][c.z]);
        fits[type][c.x][c.y][c.z] = -1;
      }
  fitness[type] = 0.0;
  obj_randomize(&fittest[type]);
}
