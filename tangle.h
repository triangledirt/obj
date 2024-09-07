#ifndef obj_tangle_h
#define obj_tangle_h

#include "obj.h"

void obj_tangle_learn(obj_t obj[], long objsz, long type);
double obj_tangle_score(obj_t obj, long type);

double obj_tangle_fit(long type);

#endif
