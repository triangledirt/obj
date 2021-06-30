#include <stdio.h>
#include "abit.h"
#include "acoord.h"
#include "ajung.h"
#include "alib.h"
#include "aobj.h"

#define DIM 16
#define ITER 128

typedef aobj_t pop_t[DIM][DIM];

static double fitness;
static double fits[DIM][DIM];
static aobj_t fittest;
static aobj_t ideal[ALIB_TYPE_COUNT];
static abit_t once = 0;

static void calcfit(pop_t pop, acoord_t *c, aobj_t objs[], long objs_size);
static double getfit(pop_t pop, acoord_t *c, aobj_t objs[], long objs_size);
static void init(pop_t pop, long type);
static void initonce();
static void meet(pop_t pop, acoord_t *a, acoord_t *b);
static void move(pop_t pop, acoord_t *a, acoord_t *b);
static void randcoord(acoord_t *c);

abit_t ajung_classify(aobj_t obj, long type)
{
  return (aobj_compare(obj, ideal[type]) > 0.75);
}

void ajung_learn(aobj_t objs[], long objs_size, long type)
{
  long iter;
  acoord_t a;
  acoord_t b;
  acoord_t i;
  double fita;
  double fitb;
  pop_t pop;
  initonce();
  init(pop, type);
  for (iter = 0; iter < ITER; iter++) {
    randcoord(&a);
    i.x = (random() % 3) - 1;
    i.y = (random() % 3) - 1;
    b.x = atool_wrapidx(a.x + i.x, DIM);
    b.y = atool_wrapidx(a.y + i.y, DIM);
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
#if ALIB_VERBOSE
  printf("type%ld ideal jng ", type);
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
  obj = pop[c->x][c->y];
  for (idx = 0; idx < objs_size; idx++) {
    tot += aobj_compare(obj, objs[idx]);
  }
  fit = tot / objs_size;
  fits[c->x][c->y] = fit;
  if (fit > fitness) {
    fittest = obj;
    fitness = fit;
  }
}

double getfit(pop_t pop, acoord_t *c, aobj_t objs[], long objs_size)
{
  if (fits[c->x][c->y] < 0) {
    calcfit(pop, c, objs, objs_size);
  }
  return fits[c->x][c->y];
}

void init(pop_t pop, long type)
{
  acoord_t c;
  for (c.x = 0; c.x < DIM; c.x++) {
    for (c.y = 0; c.y < DIM; c.y++) {
      pop[c.x][c.y] = ideal[type];
      aobj_mutate(&pop[c.x][c.y]);
      fits[c.x][c.y] = -1;
    }
  }
  aobj_randomize(&fittest);
  fitness = 0.0;
}

void initonce()
{
  long idx;
  if (!once) {
    for (idx = 0; idx < ALIB_TYPE_COUNT; idx++) {
      aobj_randomize(&ideal[idx]);
    }
    once = 1;
  }
}

void meet(pop_t pop, acoord_t *a, acoord_t *b)
{
  long bit;
  abit_t val1;
  abit_t val2;
  for (bit = 0; bit <= 32; bit++) {
    val1 = aobj_getattr(pop[a->x][a->y], bit);
    val2 = aobj_getattr(pop[b->x][b->y], bit);
    aobj_setattr(&pop[b->x][b->y], bit, val1 ^ val2);
  }
}

void move(pop_t pop, acoord_t *a, acoord_t *b)
{
  aobj_t tmp;
  tmp = pop[b->x][b->y];
  pop[b->x][b->y] = pop[a->x][a->y];
  pop[a->x][a->y] = tmp;
}

void randcoord(acoord_t *c)
{
  c->x = random() % DIM;
  c->y = random() % DIM;
}
