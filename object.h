#ifndef object_h
#define object_h

#include "bit.h"

typedef long case_object_t;

void case_object_clear(case_object_t *obj);
void case_object_randomize(case_object_t *obj);
void case_object_setfromstr(case_object_t *obj, char *str);

#define case_object_setattr(obj, idx, val) \
  if (val) { *obj |= ((long) 1 << (idx)); } \
  else { *obj &= ~((long) 1 << (idx)); }
#define case_object_getattr(obj, idx) ((obj >> (idx)) & (long) 1)

void case_object_mutate(case_object_t *obj);
void case_object_rotate(case_object_t *obj, long inc);

#define case_object_setclass(obj, val) \
  if (val) { *obj |= ((long) 1); } \
  else { *obj &= ~((long) 1); }
#define case_object_getclass(obj) (obj & (long) 1)

double case_object_comparet(case_object_t obj1, case_object_t obj2);
double case_object_compareq(case_object_t obj1, case_object_t obj2);
double case_object_comparex(case_object_t obj1, case_object_t obj2);

void case_object_print(case_object_t obj);

case_bit_t case_object_hastype(case_object_t obj, case_object_t type);

void case_object_setnum(case_object_t *obj, long startbit, long bits, long num);
long case_object_getnum(case_object_t obj, long startbit, long bits);

#endif
