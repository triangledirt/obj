#ifndef alib_h
#define alib_h

#include "abit.h"
#include "aobj.h"

void alib_notice(aobj_t obj, long type = 0);
abit_t alib_classify(aobj_t obj, long type = 0);

#endif
