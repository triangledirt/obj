#ifndef afold_h
#define afold_h

#include "abit.h"
#include "aobj.h"

void afold_learn(aobj_t objs[], long objs_size, long type);
abit_t afold_classify(aobj_t obj, long type);

#endif
