#ifndef asum_h
#define asum_h

#include "abit.h"
#include "aobj.h"

abit_t asum_classify(aobj_t obj, long type);
void asum_learn(aobj_t objs[], long objs_size, long type);

#endif
