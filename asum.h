#ifndef asum_h
#define asum_h

#include "abit.h"
#include "aobj.h"

void asum_learn(aobj_t objs[], long objs_size, long type);
abit_t asum_classify(aobj_t obj, long type);

#endif
