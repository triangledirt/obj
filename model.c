#include <stdlib.h>
#include "meetstyle.h"
#include "model.h"
#include "modelstat.h"

#define TICKS OBJ_MODEL_DIM

#define DISPLAY_GENE 0
#define MEET_GENE 1
#define MOVE_GENE 2
#define PERSONALITY_GENE 3

static obj_fit_f fitfuncs[OBJ_TYPE];
static void *contexts[OBJ_TYPE];
static enum obj_meetstyle_t meetstyles[OBJ_TYPE];
static struct obj_modelstat_t stats[OBJ_TYPE];
static obj_bool_t once = obj_bool_false;
static obj_t world[OBJ_TYPE][OBJ_MODEL_DIM][OBJ_MODEL_DIM];

static double calcfitdefault(obj_t obj, long type, void *context);
static void evolve(long ticks, long type);
static void init();
static void initworld(long type);
static void meet(long x, long y);
static void move();
static void tick();

void evolve(long ticks, long type)
{
  long i;
    for (i = 0; i < ticks; i++) {
      tick();
  }
}

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < OBJ_TYPE; type++) {
      fitfuncs[type] = obj_defaultfit;
      contexts[type] = NULL;
      initworld(type);
    }
    once = obj_bool_true;
  }
}

void initworld(long type)
{
  long x;
  long y;
  for (x = 0; x < OBJ_MODEL_DIM; x++) {
    for (y = 0; y < OBJ_MODEL_DIM; y++) {
      obj_randomize(&world[type][x][y]);
    }
  }
}

void obj_model_evolve(long type)
{
  init();
  evolve(TICKS, type);
}

void obj_model_evolvetick(long ticks, long type)
{
  init();
  evolve(ticks, type);
}

obj_t obj_model_fittest(long type)
{
  init();
  return 0;
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
  init();
  fitf = fitfuncs[type];
  ctx = contexts[type];
  do {
    obj = obj_model_random(type);
    objfit = fitf(obj, type, ctx);
  } while (objfit <= fit);
  return obj;
}

void obj_model_setfitfunc(obj_fit_f fitfunc, void *context, long type)
{
  init();
  fitfuncs[type] = fitfunc;
  contexts[type] = context;
}

void obj_model_setmeetstyle(enum obj_meetstyle_t meetstyle, long type)
{
  init();
  meetstyles[type] = meetstyle;
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

void meet()
{
}

void move(long x, long y)
{
}

void tick()
{
  long x;
  long y;
  x = random() % OBJ_MODEL_DIM;
  y = random() % OBJ_MODEL_DIM;
  move(x, y);
}
