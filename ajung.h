#ifndef ajung_h
#define ajung_h

#include "abit.h"
#include "aobj.h"

void ajung_learn(aobj_t objs[], long objs_size, long type);
abit_t ajung_classify(aobj_t obj, long type);

#endif
