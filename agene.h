#ifndef agene_h
#define agene_h

#include "abit.h"
#include "aobj.h"

abit_t agene_classify(aobj_t obj, long type);
void agene_learn(aobj_t objs[], long objs_size, long type);

#endif
