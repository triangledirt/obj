#ifndef aobj_h
#define aobj_h

typedef long aobj_t;

#define aobj_getattr(obj, idx) ((obj >> (idx)) & (long) 1)

#define aobj_setattr(obj, idx, val) \
  if (val) { *obj |= ((long) 1 << (idx)); } \
  else { *obj &= ~((long) 1 << (idx)); }

#endif
