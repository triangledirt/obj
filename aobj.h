#ifndef aobj_h
#define aobj_h

typedef uint32_t aobj_t;

#define aobj_getattr(obj, idx) ((obj >> (idx)) & (uint32_t) 1)

#define aobj_setattr(obj, idx, val) \
  if (val) { *obj |= ((uint32_t) 1 << (idx)); } \
  else { *obj &= ~((uint32_t) 1 << (idx)); }

#endif
