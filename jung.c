#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coord.h"
#include "jung.h"
#include "object.h"
#include "tool.h"
#include "toss.h"

#define DIM 8
#define ITER 8

typedef case_object_t pop_t[DIM][DIM];

static double fitness = 0.0;
static double fits[DIM][DIM];
static case_object_t fittest;
static case_object_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, coord_t *c, case_object_t objs[], long objs_size);
static void forcecalc(pop_t pop, case_object_t objs[], long objs_size);
static double getfit(pop_t pop, coord_t *c, case_object_t objs[], long objs_size);
static void init(pop_t pop, long type);
static void initonce();
static void meet(pop_t pop, coord_t *a, coord_t *b);
static void move(pop_t pop, coord_t *a, coord_t *b);
static void randcoord(coord_t *c);

case_bit_t jung_classify(case_object_t obj, long type)
{
  return case_object_comparet(obj, ideal[type]) > (0.9 * fitness);
}

void jung_learn(case_object_t objs[], long objs_size, long type)
{
  long iter;
  coord_t a;
  coord_t b;
  coord_t i;
  double fita;
  double fitb;
  pop_t pop;
  initonce();
  init(pop, type);
  for (iter = 0; iter < ITER; iter++) {
    randcoord(&a);
    i.x = (random() % 3) - 1;
    i.y = (random() % 3) - 1;
    b.x = tool_wrapidx(a.x + i.x, DIM);
    b.y = tool_wrapidx(a.y + i.y, DIM);
    fita = getfit(pop, &a, objs, objs_size);
    fitb = getfit(pop, &b, objs, objs_size);
    if (fita > fitb) {
      meet(pop, &a, &b);
      move(pop, &a, &b);
    } else {
      meet(pop, &b, &a);
      move(pop, &b, &a);
    }
  }
  forcecalc(pop, objs, objs_size);
  ideal[type] = fittest;
#if CASE_VERBOSE
  printf("type%ld ideal jng ", type);
  case_object_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}

void calcfit(pop_t pop, coord_t *c, case_object_t objs[], long objs_size)
{
  double fit;
  double tot = 0;
  long idx;
  case_object_t obj;
  obj = pop[c->x][c->y];
  for (idx = 0; idx < objs_size; idx++) {
    tot += case_object_comparet(obj, objs[idx]);
  }
  fit = tot / objs_size;
  fits[c->x][c->y] = fit;
  if (fit > fitness) {
    fittest = obj;
    fitness = fit;
  }
}

void forcecalc(pop_t pop, case_object_t objs[], long objs_size)
{
  coord_t c;
  for (c.x = 0; c.x < DIM; c.x++) {
    for (c.y = 0; c.y < DIM; c.y++) {
      if (fits[c.x][c.y] < 0) {
        calcfit(pop, &c, objs, objs_size);
      }
    }
  }
}

double getfit(pop_t pop, coord_t *c, case_object_t objs[], long objs_size)
{
  if (fits[c->x][c->y] < 0) {
    calcfit(pop, c, objs, objs_size);
  }
  return fits[c->x][c->y];
}

void init(pop_t pop, long type)
{
  coord_t c;
  for (c.x = 0; c.x < DIM; c.x++) {
    for (c.y = 0; c.y < DIM; c.y++) {
      pop[c.x][c.y] = ideal[type];
      case_object_mutate(&pop[c.x][c.y]);
      fits[c.x][c.y] = -1;
    }
  }
  case_object_randomize(&fittest);
  fitness = 0.0;
}

void initonce()
{
  long idx;
  if (!once) {
    for (idx = 0; idx < 32; idx++) {
      case_object_randomize(&ideal[idx]);
    }
    once = 1;
  }
}

void meet(pop_t pop, coord_t *a, coord_t *b)
{
  long bit;
  case_bit_t val;
  for (bit = 1; bit <= 32; bit++) {
    if (toss_coin()) {
      val = case_object_getattr(pop[a->x][a->y], bit);
      case_object_setattr(&pop[b->x][b->y], bit, val);
    } else {
      val = case_object_getattr(pop[b->x][b->y], bit);
      case_object_setattr(&pop[a->x][a->y], bit, val);
    }
  }
}

void move(pop_t pop, coord_t *a, coord_t *b)
{
  case_object_t tmp;
  tmp = pop[b->x][b->y];
  pop[b->x][b->y] = pop[a->x][a->y];
  pop[a->x][a->y] = tmp;
}

void randcoord(coord_t *c)
{
  c->x = random() % DIM;
  c->y = random() % DIM;
}
