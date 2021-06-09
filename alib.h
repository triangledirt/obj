#ifndef alib_h
#define alib_h

#include "abit.h"
#include "aobj.h"

void alib_notice(aobj_t obj, long type);
abit_t alib_classify(aobj_t obj, long type);

#endif
