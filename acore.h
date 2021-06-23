#ifndef acore_h
#define acore_h

#include "abit.h"
#include "aobj.h"

void acore_learn(aobj_t objs[], long objs_size, long type);
abit_t acore_classify(aobj_t obj, long type);

#endif
