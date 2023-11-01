#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "indx.h"
#include "model.h"
#include "modelstat.h"
#include "movegene.h"
#include "selfgene.h"
#include "showgene.h"
#include "talkgene.h"

#define MOVE_GENE 0
#define SELF_GENE 8
#define SHOW_GENE 16
#define TALK_GENE 24
#define TICKS OBJ_MODEL_DIM

static obj_fit_f fitfuncs[OBJ_TYPE];
static void *contexts[OBJ_TYPE];
static struct obj_modelstat_t stats[OBJ_TYPE];
static enum obj_bool_t once = obj_bool_false;
static obj_t world[OBJ_TYPE][OBJ_MODEL_DIM][OBJ_MODEL_DIM];
static obj_t fittest[OBJ_TYPE];
static double fittestfit[OBJ_TYPE];
static double fit[OBJ_TYPE][OBJ_MODEL_DIM][OBJ_MODEL_DIM];

static void calcfit(obj_t obj, long x, long y, long type, obj_fit_f fitfunc, void *context);
static long calcmovecoord(long coord, long offset);
static enum obj_bool_t conquers(obj_t obj1, long x1, long y1, obj_t obj2, long x2, long y2, long type);
static void forcecalc(long type, obj_fit_f fitfunc, void *context);
static double getfit(obj_t obj, long x, long y, long type, obj_fit_f fitfunc, void *context);
static void init();
static void initworld(long type);
static void move(long x, long y, long type);
static void swap(long x1, long y1, long x2, long y2, long type);
static void talk(obj_t *obj1, obj_t *obj2, obj_bit_t narcissist, long type);
static void tick(long type);

void calcfit(obj_t obj, long x, long y, long type, obj_fit_f fitfunc, void *context)
{
  double fitness;
  fitness = fitfunc(obj, type, context);
  fit[type][x][y] = fitness;
  if (fitness > fittestfit[type]) {
    fittest[type] = obj;
    fittestfit[type] = fitness;
  }
}

long calcmovecoord(long coord, long offset)
{
  return obj_indx_wrap(coord + offset, OBJ_MODEL_DIM);
}

enum obj_bool_t conquers(obj_t obj1, long x1, long y1, obj_t obj2, long x2, long y2, long type)
{
  double fit1;
  double fit2;
  obj_fit_f fitfunc;
  void *context;
  enum obj_bool_t conquers;
  fitfunc = fitfuncs[type];
  if (fitfunc) {
    context = contexts[type];
    fit1 = getfit(obj1, x1, y1, type, fitfunc, context);
    fit2 = getfit(obj2, x2, y2, type, fitfunc, context);
    conquers = (fit1 > fit2) ? obj_bool_true : obj_bool_false;
  } else {
    conquers = obj_bool_true;
  }
  return conquers;
}

void forcecalc(long type, obj_fit_f fitfunc, void *context)
{
  long x;
  long y;
  obj_t obj;
  for (x = 0; x < OBJ_MODEL_DIM; x++)
    for (y = 0; y < OBJ_MODEL_DIM; y++)
      if (fit[type][x][y] < 0) {
        obj = world[type][x][y];
        calcfit(obj, x, y, type, fitfunc, context);
      }
}

double getfit(obj_t obj, long x, long y, long type, obj_fit_f fitfunc, void *context)
{
  if (fit[type][x][y] < 0)
    calcfit(obj, x, y, type, fitfunc, context);
  return fit[type][x][y];
}

void init()
{
  long type;
  if (!once) {
    srandom(time(NULL));
    for (type = 0; type < OBJ_TYPE; type++) {
      fitfuncs[type] = NULL;
      contexts[type] = NULL;
      obj_randomize(&fittest[type]);
      fittestfit[type] = -1;
      initworld(type);
      obj_modelstat_reset(&stats[type]);
    }
    once = obj_bool_true;
  }
}

void initworld(long type)
{
  long x;
  long y;
  for (x = 0; x < OBJ_MODEL_DIM; x++)
    for (y = 0; y < OBJ_MODEL_DIM; y++) {
      obj_randomize(&world[type][x][y]);
      fit[type][x][y] = -1;
    }
}

void obj_model_evolve(long type)
{
  long i;
  init();
  for (i = 0; i < TICKS; i++)
    tick(type);
  obj_model_print(10, 6, type);
}

struct obj_fit_t obj_model_fittest(long type)
{
  struct obj_fit_t fit;
  init();
  fit.obj = fittest[type];
  fit.fit = fittestfit[type];
  return fit;
}

obj_bit_t obj_model_getclass(long x, long y, long type)
{
  obj_t obj;
  obj = world[type][x][y];
  return obj_getclass(obj);
}

void obj_model_insert(obj_t obj, long type)
{
  long x;
  long y;
  init();
  x = random() % OBJ_MODEL_DIM;
  y = random() % OBJ_MODEL_DIM;
  world[type][x][y] = obj;
}

void obj_model_print(long width, long height, long type)
{
  long x;
  long y;
  obj_t obj;
  struct obj_showgene_t showgene;
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      obj = world[type][x][y];
      obj_showgene_parse(&showgene, obj, SHOW_GENE);
      printf("%c", (showgene.face % 26) + 97);
    }
    printf("\n");
  }
  printf("\n");
}

obj_t obj_model_random(long type)
{
  long x;
  long y;
  init();
  x = random() % OBJ_MODEL_DIM;
  y = random() % OBJ_MODEL_DIM;
  return world[type][x][y];
}

obj_t obj_model_randomabove(double fit, long type)
{
  double objfit;
  obj_fit_f fitf;
  void *ctx;
  obj_t obj;
  long maxtries = OBJ_MODEL_DIM;
  long tries = 0;
  init();
  fitf = fitfuncs[type];
  ctx = contexts[type];
  do {
    obj = obj_model_random(type);
    objfit = fitf(obj, type, ctx);
    tries++;
  } while ((objfit <= fit) && (tries < maxtries));
  return obj;
}

void obj_model_setfitfunc(obj_fit_f fitfunc, void *context, long type)
{
  init();
  fitfuncs[type] = fitfunc;
  contexts[type] = context;
}

struct obj_modelstat_t *obj_model_stat(long type)
{
  init();
  return &stats[type];
}

void obj_model_resetstat(long type)
{
  init();
  obj_modelstat_reset(&stats[type]);
}

void swap(long x1, long y1, long x2, long y2, long type)
{
  obj_t temp;
  temp = world[type][x1][y1];
  world[type][x1][y1] = world[type][x2][y2];
  world[type][x2][y2] = temp;
  stats[type].swaps++;
}

void talk(obj_t *obj1, obj_t *obj2, obj_bit_t narcissist, long type)
{
  struct obj_talkgene_t talkgene;
  long i;
  long j;
  obj_bit_t bit;
  obj_talkgene_parse(&talkgene, TALK_GENE, *obj1);
  for (i = talkgene.send.start; i < talkgene.send.length; i++) {
    j = obj_indx_wrap(i, OBJ);
    bit = obj_getattr(*obj1, j);
    obj_setattr(obj2, j, bit);
  }
  if (!narcissist)
    for (i = talkgene.receive.start; i < talkgene.receive.length; i++) {
      j = obj_indx_wrap(i, OBJ);
      bit = obj_getattr(*obj2, j);
      obj_setattr(obj1, j, bit);
    }
  stats[type].talks++;
}

void tick(long type)
{
  long x;
  long y;
  long targetx;
  long targety;
  obj_t *obj;
  obj_t *target;
  struct obj_movegene_t movegene;
  struct obj_selfgene_t selfgene;
  struct obj_selfgene_t targetselfgene;
  x = random() % OBJ_MODEL_DIM;
  y = random() % OBJ_MODEL_DIM;
  obj = &world[type][x][y];
  obj_selfgene_parse(&selfgene, SELF_GENE, *obj);
  if (selfgene.extrovert) {
    obj_movegene_parse(&movegene, MOVE_GENE, *obj);
    targetx = calcmovecoord(x, movegene.xoffset);
    targety = calcmovecoord(y, movegene.yoffset);
    target = &world[type][targetx][targety];
    obj_selfgene_parse(&targetselfgene, SELF_GENE, *target);
    if ((!selfgene.racist) || (selfgene.color == targetselfgene.color))
      if (conquers(*obj, x, y, *target, targetx, targety, type)) {
	talk(obj, target, selfgene.narcissist, type);
	if (!selfgene.narcissist)
	  swap(x, y, targetx, targety, type);
      }
  }
  stats[type].ticks++;
}
