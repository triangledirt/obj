#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coord.h"
#include "jung.h"
#include "obj.h"
#include "tool.h"
#include "toss.h"

#define DIM 8
#define ITER 8

typedef case_obj_t pop_t[DIM][DIM];

static double fitness = 0.0;
static double fits[DIM][DIM];
static case_obj_t fittest;
static case_obj_t ideal[32];
static case_bit_t once = 0;

static void calcfit(pop_t pop, coord_t *c, case_obj_t objs[], long objssz);
static void forcecalc(pop_t pop, case_obj_t objs[], long objssz);
static double getfit(pop_t pop, coord_t *c, case_obj_t objs[], long objssz);
static void init(pop_t pop, long type);
static void initonce();
static void meet(pop_t pop, coord_t *a, coord_t *b);
static void move(pop_t pop, coord_t *a, coord_t *b);
static void randcoord(coord_t *c);

case_bit_t jung_classify(case_obj_t obj, long type)
{
  return case_obj_comparet(obj, ideal[type]) > (0.9 * fitness);
}

void jung_learn(case_obj_t objs[], long objssz, long type)
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
    fita = getfit(pop, &a, objs, objssz);
    fitb = getfit(pop, &b, objs, objssz);
    if (fita > fitb) {
      meet(pop, &a, &b);
      move(pop, &a, &b);
    } else {
      meet(pop, &b, &a);
      move(pop, &b, &a);
    }
  }
  forcecalc(pop, objs, objssz);
  ideal[type] = fittest;
#if CASE_VERBOSE
  printf("type%ld ideal jng ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}

void calcfit(pop_t pop, coord_t *c, case_obj_t objs[], long objssz)
{
  double fit;
  double tot = 0;
  long idx;
  case_obj_t obj;
  obj = pop[c->x][c->y];
  for (idx = 0; idx < objssz; idx++)
    if (toss_coin())
      tot += case_obj_comparet(obj, objs[idx]);
  fit = tot / (objssz / 2);
  fits[c->x][c->y] = fit;
  if (fit > fitness) {
    fittest = obj;
    fitness = fit;
  }
}

void forcecalc(pop_t pop, case_obj_t objs[], long objssz)
{
  coord_t c;
  for (c.x = 0; c.x < DIM; c.x++)
    for (c.y = 0; c.y < DIM; c.y++)
      if (fits[c.x][c.y] < 0)
        calcfit(pop, &c, objs, objssz);
}

double getfit(pop_t pop, coord_t *c, case_obj_t objs[], long objssz)
{
  if (fits[c->x][c->y] < 0)
    calcfit(pop, c, objs, objssz);
  return fits[c->x][c->y];
}

void init(pop_t pop, long type)
{
  coord_t c;
  for (c.x = 0; c.x < DIM; c.x++)
    for (c.y = 0; c.y < DIM; c.y++) {
      pop[c.x][c.y] = ideal[type];
      case_obj_mutate(&pop[c.x][c.y]);
      fits[c.x][c.y] = -1;
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

void meet(pop_t pop, coord_t *a, coord_t *b)
{
  long bit;
  case_bit_t val;
  for (bit = 1; bit <= 32; bit++) {
    if (toss_coin()) {
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
