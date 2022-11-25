#include "model.h"

#define TICKS OBJ_MODEL_DIM

static obj_fit_f fitfuncs[OBJ_TYPE];
static obj_bool_t once = obj_bool_false;

static double calcfitdefault(obj_t obj, long type, void *context);
static void init();
static void tick();

void init()
{
  long type;
  for (type = 0; type < OBJ_TYPE; type++) {
    fitfuncs[type] = obj_defaultfit;
  }
}

void obj_model_evolve(long type)
{
}

void obj_model_evolvetick(long ticks, long type)
{
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
}

void obj_model_setmeetstyle(enum obj_meetstyle_t meetstyle, long type)
{
}

struct obj_modelstat_t *obj_model_stat(long type)
{
}

void obj_model_resetstat(long type)
{
}
