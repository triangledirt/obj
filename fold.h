#ifndef fold_h
#define fold_h

#include "bit.h"
#include "obj.h"

void fold_learn(case_obj_t obj[], long objsz, long type);
double fold_classify(case_obj_t obj, long type);

#endif
