#ifndef abits_h
#define abits_h

#include "abit.h"
#include "aobj.h"

abit_t abits_classify(aobj_t obj);
void abits_learn(aobj_t objs[], long objs_size);

#endif
