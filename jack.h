#ifndef jack_h
#define jack_h

#include "obj.h"

void jack_learn(case_obj_t obj[], long objsz, long type);
double jack_score(case_obj_t obj, long type);

#endif
