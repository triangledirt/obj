#ifndef alib_h
#define alib_h

#include "abit.h"
#include "aobj.h"

#define ALIB_TYPE_COUNT 8

void alib_observe(aobj_t obj, long type);
abit_t alib_classify(aobj_t obj, long type);

#endif
