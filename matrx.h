#ifndef matrx_h
#define matrx_h

#include "bit.h"
#include "obj.h"

void matrx_learn(case_obj_t obj[], long objsz, long type);
case_bit_t matrx_classify(case_obj_t obj, long type);

#endif
