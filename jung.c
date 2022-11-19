#include <stdio.h>
#include "bit.h"
#include "class.h"
#include "coin.h"
#include "coord.h"
#include "indx.h"
#include "jung.h"
#include "obj.h"

#define DIM 8
#define ITER 8

typedef obj_t pop_t[DIM][DIM];

static double fitness[OBJ_CLASS_TYPE];
static double fits[OBJ_CLASS_TYPE][DIM][DIM];
static obj_t fittest[OBJ_CLASS_TYPE];
static obj_t ideal[OBJ_CLASS_TYPE];
static obj_bool_t once = obj_bool_false;

static void calcfit(pop_t pop, obj_coord_t *c, obj_t obj[], long objsz, long type);
static void forcecalc(pop_t pop, obj_t obj[], long objsz, long type);
static double getfit(pop_t pop, obj_coord_t *c, obj_t obj[], long objsz, long type);
static void init();
static void meet(pop_t pop, obj_coord_t *a, obj_coord_t *b);
static void move(pop_t pop, obj_coord_t *a, obj_coord_t *b);
static void randcoord(obj_coord_t *c);
static void reset(pop_t pop, long type);

void calcfit(pop_t pop, obj_coord_t *c, obj_t obj[], long objsz, long type)
{
  double fit;
  double tot = 0;
  long i;
  obj_t o;
  o = pop[c->x][c->y];
  for (i = 0; i < objsz; i++)
    if (obj_coin_toss())
      tot += obj_compare(o, obj[i]);
  fit = tot / (objsz / 2);
  fits[type][c->x][c->y] = fit;
  if (fit > fitness[type]) {
    fittest[type] = o;
    fitness[type] = fit;
  }
}

void forcecalc(pop_t pop, obj_t obj[], long objsz, long type)
{
  obj_coord_t c;
  for (c.x = 0; c.x < DIM; c.x++)
    for (c.y = 0; c.y < DIM; c.y++)
      if (fits[type][c.x][c.y] < 0)
        calcfit(pop, &c, obj, objsz, type);
}

double getfit(pop_t pop, obj_coord_t *c, obj_t obj[], long objsz, long type)
{
  if (fits[c->x][c->y] < 0)
    calcfit(pop, c, obj, objsz, type);
  return fits[type][c->x][c->y];
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

void obj_jung_learn(obj_t obj[], long objsz, long type)
{
  long iter;
  obj_coord_t a;
  obj_coord_t b;
  obj_coord_t i;
  double fita;
  double fitb;
  pop_t pop;
  init();
  reset(pop, type);
  for (iter = 0; iter < ITER; iter++) {
    randcoord(&a);
    i.x = (random() % 3) - 1;
    i.y = (random() % 3) - 1;
    b.x = obj_indx_wrap(a.x + i.x, DIM);
    b.y = obj_indx_wrap(a.y + i.y, DIM);
    fita = getfit(pop, &a, obj, objsz, type);
    fitb = getfit(pop, &b, obj, objsz, type);
    if (fita > fitb) {
      meet(pop, &a, &b);
      move(pop, &a, &b);
    } else {
      meet(pop, &b, &a);
      move(pop, &b, &a);
    }
  }
  forcecalc(pop, obj, objsz, type);
  ideal[type] = fittest[type];
#if OBJ_VERBOSE
  printf("type%02ld ideal jng ", type);
  obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double obj_jung_score(obj_t obj, long type)
{
  init();
  return obj_compare(obj, ideal[type]);
}

void meet(pop_t pop, obj_coord_t *a, obj_coord_t *b)
{
  long bit;
  obj_bit_t val;
  for (bit = 1; bit <= OBJ; bit++) {
    if (obj_coin_toss()) {
      val = obj_getattr(pop[a->x][a->y], bit);
      obj_setattr(&pop[b->x][b->y], bit, val);
    } else {
      val = obj_getattr(pop[b->x][b->y], bit);
      obj_setattr(&pop[a->x][a->y], bit, val);
    }
  }
}

void move(pop_t pop, obj_coord_t *a, obj_coord_t *b)
{
  obj_t tmp;
  tmp = pop[b->x][b->y];
  pop[b->x][b->y] = pop[a->x][a->y];
  pop[a->x][a->y] = tmp;
}

void randcoord(obj_coord_t *c)
{
  c->x = random() % DIM;
  c->y = random() % DIM;
}

void reset(pop_t pop, long type)
{
  obj_coord_t c;
  for (c.x = 0; c.x < DIM; c.x++)
    for (c.y = 0; c.y < DIM; c.y++) {
      pop[c.x][c.y] = ideal[type];
      obj_mutate(&pop[c.x][c.y]);
      fits[type][c.x][c.y] = -1;
    }
  fitness[type] = 0.0;
  obj_randomize(&fittest[type]);
}
