#ifndef moire_h
#define moire_h

#include "obj.h"

void moire_learn(case_obj_t obj[], long objsz, long type);
double moire_score(case_obj_t obj, long type);

#endif
