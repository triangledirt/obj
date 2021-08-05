#ifndef aobj_h
#define aobj_h

#include "aobj.h"

typedef long aobj_t;

void aobj_clear(aobj_t *obj);
void aobj_randomize(aobj_t *obj);
void aobj_setfromstr(aobj_t *obj, char *str);

#define aobj_setattr(obj, idx, val) \
  if (val) { *obj |= ((long) 1 << (idx)); } \
  else { *obj &= ~((long) 1 << (idx)); }
#define aobj_getattr(obj, idx) ((obj >> (idx)) & (long) 1)

void aobj_mutate(aobj_t *obj);
void aobj_rotate(aobj_t *obj, long inc);

#define aobj_setclass(obj, val) \
  if (val) { *obj |= ((long) 1); } \
  else { *obj &= ~((long) 1); }
#define aobj_getclass(obj) (obj & (long) 1)

double aobj_comparet(aobj_t obj1, aobj_t obj2);
double aobj_compareq(aobj_t obj1, aobj_t obj2);
double aobj_comparex(aobj_t obj1, aobj_t obj2);

void aobj_print(aobj_t obj);

abit_t aobj_hastype(aobj_t obj, aobj_t type);

void aobj_setnum(aobj_t *obj, long startbit, long bits, long num);
long aobj_getnum(aobj_t obj, long startbit, long bits);

#endif
