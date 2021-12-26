#ifndef sum_h
#define sum_h

#include "bit.h"
#include "obj.h"

void sum_learn(case_obj_t obj[], long objsz, long type);
case_bit_t sum_classify(case_obj_t obj, long type);

#endif
