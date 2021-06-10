#ifndef alib_h
#define alib_h

#include "abit.h"
#include "aobj.h"

#define ALIB_TYPE_COUNT 8
#define ALIB_VERBOSE 1

abit_t alib_classify(aobj_t obj, long type);
void alib_observe(aobj_t obj, long type);

#endif
