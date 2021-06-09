#ifndef alib_h
#define alib_h

#include "abit.h"
#include "aobj.h"

#define ALIB_TYPES_MAX 8

void alib_notice(aobj_t obj, long type);
abit_t alib_classify(aobj_t obj, long type);

#endif
