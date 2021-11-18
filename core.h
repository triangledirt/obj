#ifndef core_h
#define core_h

#include "bit.h"
#include "object.h"

void core_learn(case_object_t objs[], long objs_size, long type);
case_bit_t core_classify(case_object_t obj, long type);

#endif
