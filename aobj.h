#ifndef aobj_h
#define aobj_h

#include "aobj.h"

typedef long aobj_t;

void aobj_clear(aobj_t *obj);
void aobj_randomize(aobj_t *obj);
void aobj_mutate(aobj_t *obj);

#define aobj_getattr(obj, idx) ((obj >> (idx)) & (long) 1)
#define aobj_setattr(obj, idx, val) \
  if (val) { *obj |= ((long) 1 << (idx)); } \
  else { *obj &= ~((long) 1 << (idx)); }

#define aobj_getclass(obj) (obj & (long) 1)
#define aobj_setclass(obj, val) \
  if (val) { *obj |= ((long) 1); } \
  else { *obj &= ~((long) 1); }

double aobj_compare(aobj_t obj1, aobj_t obj2);

void aobj_print(aobj_t obj);

#endif
