#ifndef core_h
#define core_h

#include "bit.h"
#include "obj.h"

void core_learn(case_obj_t objs[], long objs_size, long type);
case_bit_t core_classify(case_obj_t obj, long type);

#endif
