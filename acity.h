#ifndef acity_h
#define acity_h

#include "abit.h"
#include "aobj.h"

void acity_learn(aobj_t objs[], long objs_size, long type);
abit_t acity_classify(aobj_t obj, long type);

#endif
