#ifndef fold_h
#define fold_h

#include "bit.h"
#include "obj.h"

void fold_learn(case_obj_t objs[], long objssze, long type);
case_bit_t fold_classify(case_obj_t obj, long type);

#endif
