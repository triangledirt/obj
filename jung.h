#ifndef jung_h
#define jung_h

#include "bit.h"
#include "obj.h"

void jung_learn(case_obj_t objs[], long objs_size, long type);
case_bit_t jung_classify(case_obj_t obj, long type);

#endif
