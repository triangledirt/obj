#ifndef fold_h
#define fold_h

#include "obj.h"

void fold_learn(case_obj_t obj[], long objsz, long type);
double fold_score(case_obj_t obj, long type);

#endif
