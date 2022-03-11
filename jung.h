#ifndef jung_h
#define jung_h

#include "bit.h"
#include "obj.h"

void jung_learn(case_obj_t obj[], long objsz, long type);
double jung_score(case_obj_t obj, long type);

#endif
