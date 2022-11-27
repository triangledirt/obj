#include "meetstyle.h"
#include "model.h"
#include "modelstat.h"

#define TICKS OBJ_MODEL_DIM

static obj_fit_f fitfuncs[OBJ_TYPE];
static void *contexts[OBJ_TYPE];
static enum obj_meetstyle_t meetstyles[OBJ_TYPE];
static struct obj_modelstat_t stats[OBJ_TYPE];
static obj_bool_t once = obj_bool_false;

static double calcfitdefault(obj_t obj, long type, void *context);
static void evolve(long ticks, long type);
static void init();
static void tick();

void evolve(long ticks, long type)
{
  long tick;
    for (tick = 0; tick < ticks; tick++) {
  }
}

void init()
{
  long type;
  for (type = 0; type < OBJ_TYPE; type++) {
    fitfuncs[type] = obj_defaultfit;
  }
}

void obj_model_evolve(long type)
{
  evolve(TICKS, type);
}

void obj_model_evolvetick(long ticks, long type)
{
  evolve(ticks, type);
}

obj_t obj_model_fittest(long type)
{
  return 0;
}

void obj_model_insert(obj_t obj, long type)
{
}

obj_t obj_model_random(long type)
{
  return 0;
}

obj_t obj_model_randomabove(double fitness, long type)
{
  return 0;
}

void obj_model_setfitfunc(obj_fit_f fitfunc, void *context, long type)
{
  fitfuncs[type] = fitfunc;
  contexts[type] = context;
}

void obj_model_setmeetstyle(enum obj_meetstyle_t meetstyle, long type)
{
  meetstyles[type] = meetstyle;
}

struct obj_modelstat_t *obj_model_stat(long type)
{
  return &stats[type];
}

void obj_model_resetstat(long type)
{
  obj_modelstat_reset(&stats[type]);
}
