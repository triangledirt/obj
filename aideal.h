#ifndef abits_h
#define abits_h

#include "abit.h"
#include "aobj.h"

abit_t aideal_classify(aobj_t obj, long type);
void aideal_learn(aobj_t objs[], long objs_size, long type);

#endif
