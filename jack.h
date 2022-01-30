#ifndef jack_h
#define jack_h

#include "bit.h"
#include "obj.h"

void jack_learn(case_obj_t obj[], long objsz, long type);
case_bit_t jack_classify(case_obj_t obj, long type);

#endif
