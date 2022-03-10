#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "coord.h"
#include "indx.h"
#include "jung.h"
#include "obj.h"

#define DIM 8
#define ITER 8

typedef case_obj_t pop_t[DIM][DIM];

static double fitness[32];
static double fits[32][DIM][DIM];
static case_obj_t fittest[32];
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, coord_t *c, case_obj_t obj[], long objsz, long type);

static void forcecalc(pop_t pop, case_obj_t obj[], long objsz, long type);

static double getfit(pop_t pop, coord_t *c, case_obj_t obj[], long objsz, long type);

static void initonce();
static void meet(pop_t pop, coord_t *a, coord_t *b);
static void move(pop_t pop, coord_t *a, coord_t *b);
static void randcoord(coord_t *c);
static void reset(pop_t pop, long type);

void calcfit(pop_t pop, coord_t *c, case_obj_t obj[], long objsz, long type)
{
  double fit;
  double tot = 0;
  long i;
  case_obj_t o;
  o = pop[c->x][c->y];
  for (i = 0; i < objsz; i++)
    if (coin_toss())
      tot += case_obj_comparetypes(o, obj[i]);
  fit = tot / (objsz / 2);
  fits[type][c->x][c->y] = fit;
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
      if (fits[type][c.x][c.y] < 0)
        calcfit(pop, &c, obj, objsz, type);
}

double getfit(pop_t pop, coord_t *c, case_obj_t obj[], long objsz, long type)
{
  if (fits[c->x][c->y] < 0)
    calcfit(pop, c, obj, objsz, type);
  return fits[type][c->x][c->y];
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

double jung_classify(case_obj_t obj, long type)
{
  initonce();
  return case_obj_comparetypes(obj, ideal[type]);
}

void jung_learn(case_obj_t obj[], long objsz, long type)
{
  long iter;
  coord_t a;
  coord_t b;
  coord_t i;
  double fita;
  double fitb;
  pop_t pop;
  initonce();
  reset(pop, type);
  for (iter = 0; iter < ITER; iter++) {
    randcoord(&a);
    i.x = (random() % 3) - 1;
    i.y = (random() % 3) - 1;
    b.x = indx_wrap(a.x + i.x, DIM);
    b.y = indx_wrap(a.y + i.y, DIM);
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
#if CASE_VERBOSE
  printf("type%ld ideal jng ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

void meet(pop_t pop, coord_t *a, coord_t *b)
{
  long bit;
  case_bit_t val;
  for (bit = 1; bit <= 32; bit++) {
    if (coin_toss()) {
      val = case_obj_getattr(pop[a->x][a->y], bit);
      case_obj_setattr(&pop[b->x][b->y], bit, val);
    } else {
      val = case_obj_getattr(pop[b->x][b->y], bit);
      case_obj_setattr(&pop[a->x][a->y], bit, val);
    }
  }
}

void move(pop_t pop, coord_t *a, coord_t *b)
{
  case_obj_t tmp;
  tmp = pop[b->x][b->y];
  pop[b->x][b->y] = pop[a->x][a->y];
  pop[a->x][a->y] = tmp;
}

void randcoord(coord_t *c)
{
  c->x = random() % DIM;
  c->y = random() % DIM;
}

void reset(pop_t pop, long type)
{
  coord_t c;
  for (c.x = 0; c.x < DIM; c.x++)
    for (c.y = 0; c.y < DIM; c.y++) {
      pop[c.x][c.y] = ideal[type];
      case_obj_mutate(&pop[c.x][c.y]);
      fits[type][c.x][c.y] = -1;
    }
  fitness[type] = 0.0;
  case_obj_randomize(&fittest[type]);
}
