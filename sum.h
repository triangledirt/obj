#ifndef sum_h
#define sum_h

#include "bit.h"
#include "object.h"

void sum_learn(case_object_t objs[], long objs_size, long type);
case_bit_t sum_classify(case_object_t obj, long type);

#endif
