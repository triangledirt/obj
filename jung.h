#ifndef jung_h
#define jung_h

#include "bit.h"
#include "object.h"

void jung_learn(case_object_t objs[], long objs_size, long type);
case_bit_t jung_classify(case_object_t obj, long type);

#endif
