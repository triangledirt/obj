#ifndef core_h
#define core_h

#include "bit.h"
#include "obj.h"

void core_learn(case_obj_t obj[], long objsz, long type);
double core_classify(case_obj_t obj, long type);

#endif
