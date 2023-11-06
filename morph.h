#ifndef obj_morph_h
#define obj_morph_h

#include "obj.h"

void obj_morph_learn(obj_t obj[], long objsz, long type);
double obj_morph_score(obj_t obj, long type);

double obj_morph_fit(long type);

#endif
