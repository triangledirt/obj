#ifndef abits_h
#define abits_h

#include "abit.h"
#include "aobj.h"

abit_t apole_classify(aobj_t obj);
void apole_learn(aobj_t objs[], long objs_size, long type);

#endif
