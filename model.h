#ifndef obj_model_h
#define obj_model_h

#include "modelstat.h"
#include "obj.h"

#define OBJ_MODEL_DIM 128
#define OBJ_MODEL_TYPE OBJ

typedef double (*calcfit_f)(obj_t);
// move this to obj.h ??

void obj_model_setcalcfitfunc(long type, calcfit_f calcfitfunc);

void obj_model_insert(obj_t obj, long type);
void obj_model_evolve(long type);

obj_t obj_model_fittest(long type);
obj_t obj_model_random(long type);
obj_t obj_model_randomabove(double fitness, long type);

/*  change model params  */

void obj_model_resetstat(long type);
struct obj_modelstat_t *obj_model_stat(long type);

#endif
