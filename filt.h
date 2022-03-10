#ifndef filt_h
#define filt_h

#include "bit.h"
#include "obj.h"

void filt_learn(case_obj_t obj[], long objsz, long type);
case_bit_t filt_classify(case_obj_t obj, long type);
double filt_classifydouble(case_obj_t obj, long type);

#endif
