#ifndef obj_model_h
#define obj_model_h

#include "meetstyle.h"
#include "modelstat.h"
#include "obj.h"

#define OBJ_MODEL_DIM 128
#define OBJ_MODEL_TYPE OBJ

void obj_model_setfitfunc(obj_fit_f fitfunc, void *context, long type);

void obj_model_insert(obj_t obj, long type);
void obj_model_evolve(long type);
void obj_model_evolvetick(long ticks, long type);

obj_t obj_model_fittest(long type);
obj_t obj_model_random(long type);
obj_t obj_model_randomabove(double fit, long type);

void obj_model_setmeetstyle(enum obj_meetstyle_t meetstyle, long type);

void obj_model_resetstat(long type);
struct obj_modelstat_t *obj_model_stat(long type);

#endif
