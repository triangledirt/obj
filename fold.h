#ifndef obj_fold_h
#define obj_fold_h

#include "obj.h"

void obj_fold_learn(obj_t obj[], long objsz, long type);
double obj_fold_score(obj_t obj, long type);

double obj_fold_fitness(long type);

#endif
