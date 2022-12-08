#ifndef obj_sum_h
#define obj_sum_h

#include "obj.h"

void obj_sum_learn(obj_t obj[], long objsz, long type);
double obj_sum_score(obj_t obj, long type);

double obj_sum_fitness(long type);

#endif
