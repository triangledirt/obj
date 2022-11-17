#ifndef obj_filt_h
#define obj_filt_h

#include "obj.h"

void obj_filt_learn(obj_t obj[], long objsz, long type);
double obj_filt_score(obj_t obj, long type);

#endif
