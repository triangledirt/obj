#ifndef matrix_h
#define matrix_h

#include "bit.h"
#include "obj.h"

void matrix_learn(case_obj_t obj[], long objsz, long type);
case_bit_t matrix_classify(case_obj_t obj, long type);

#endif
