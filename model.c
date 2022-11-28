#include <stdlib.h>
#include "displaygene.h"
#include "meetgene.h"
#include "meetstyle.h"
#include "model.h"
#include "modelstat.h"
#include "movegene.h"
#include "persongene.h"

#define TICKS OBJ_MODEL_DIM

#define DISPLAY_GENE 0
#define MEET_GENE 8
#define MOVE_GENE 32
#define PERSON_GENE 40

static obj_fit_f fitfuncs[OBJ_TYPE];
static void *contexts[OBJ_TYPE];
static enum obj_meetstyle_t meetstyles[OBJ_TYPE];
static struct obj_modelstat_t stats[OBJ_TYPE];
static obj_bool_t once = obj_bool_false;
static obj_t world[OBJ_TYPE][OBJ_MODEL_DIM][OBJ_MODEL_DIM];

static double calcfitdefault(obj_t obj, long type, void *context);
static long calcmovecoord(long coord, obj_bit_t offset);
static void evolve(long ticks, long type);
static void init();
static void initworld(long type);
static void meet(long x, long y, long targtx, long targety, long type);
static void move(long x, long y, long type);
static void swap(long x1, long y1, long x2, long y2, long type);
static void tick(long type);

long calcmovecoord(long coord, obj_bit_t offset)
{
  return (offset) ? coord - 1 : coord + 1;
}

void evolve(long ticks, long type)
{
  long i;
    for (i = 0; i < ticks; i++) {
      tick(type);
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
  for (x = 0; x < OBJ_MODEL_DIM; x++)
    for (y = 0; y < OBJ_MODEL_DIM; y++)
      obj_randomize(&world[type][x][y]);
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

void meet(long x, long y, long targetx, long targety, long type)
{
  struct obj_meetgene_t meetgene;
  obj_t thisobj;
  obj_t thatobj;
  thisobj = world[type][x][y];
  thatobj = world[type][targetx][targety];
  obj_meetgene_parse(&meetgene, MEET_GENE, thisobj);
  ;;
}

void move(long x, long y, long type)
{
  obj_t obj;
  struct obj_movegene_t movegene;
  struct obj_persongene_t persongene;
  long targetx;
  long targety;
  obj = world[type][x][y];
  obj_movegene_parse(&movegene, MOVE_GENE, obj);
  obj_persongene_parse(&persongene, PERSON_GENE, obj);
  if (persongene.extrovert) {
    targetx = calcmovecoord(x, movegene.xoffset);
    targety = calcmovecoord(y, movegene.yoffset);
    meet(x, y, targetx, targety, type);
    swap(x, y, targetx, targety, type);
  }
}

void swap(long x1, long y1, long x2, long y2, long type)
{
  obj_t temp;
  temp = world[type][x1][y1];
  world[type][x1][y1] = world[type][x2][y2];
  world[type][x2][y2] = temp;
}

void tick(long type)
{
  long x;
  long y;
  x = random() % OBJ_MODEL_DIM;
  y = random() % OBJ_MODEL_DIM;
  move(x, y, type);
}
