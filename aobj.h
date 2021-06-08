#ifndef aobj_h
#define aobj_h

typedef unsigned long aobj_t;

#define aobj_getattr(obj, idx) ((obj >> (idx)) & (unsigned long) 1)

#define aobj_setattr(obj, idx, val) \
  if (val) { *obj |= ((unsigned long) 1 << (idx)); } \
  else { *obj &= ~((unsigned long) 1 << (idx)); }

#endif
