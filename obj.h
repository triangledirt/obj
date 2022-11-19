#ifndef obj_h
#define obj_h

#include "bool.h"

#define OBJ_VERBOSE 1
#define OBJ_XVERBOSE 1

#define OBJ 64
typedef long long obj_t;
#define OBJ_CLEAR 0

void obj_clear(obj_t *obj);
void obj_fill(obj_t *obj);
void obj_randomize(obj_t *obj);
void obj_setfromstr(obj_t *obj, char str[OBJ]);

#define obj_setattr(obj, indx, val) if (val) { *obj |= ((long long) 1 << indx); } else { *obj &= ~((long long) 1 << indx); }
#define obj_getattr(obj, indx) ((obj >> indx) & (long long) 1)
#define obj_setclass(obj, val) if (val) { *obj |= ((long long) 1); } else { *obj &= ~((long long) 1); }
#define obj_getclass(obj) (obj & (long long) 1)

double obj_comparebox(obj_t obj1, obj_t obj2);
double obj_compareequal(obj_t obj1, obj_t obj2);
double obj_comparefocus(obj_t obj1, obj_t obj2);
double obj_compareoblivion(obj_t obj1, obj_t obj2);
double obj_comparetypes(obj_t obj1, obj_t obj2);
double obj_comparexor(obj_t obj1, obj_t obj2);
#define obj_compare(obj1, obj2) obj_comparetypes(obj1, obj2)

obj_bool_t obj_hastype(obj_t obj, obj_t type);
void obj_print(obj_t obj);

void obj_obscureclass(obj_t *obj);
void obj_mutate(obj_t *obj);
void obj_rotate(obj_t *obj, long inc);

void obj_setnum(obj_t *obj, long startbit, long bits, long long num);
long long obj_getnum(obj_t obj, long startbit, long bits);

long obj_smash(obj_t obj1, obj_t obj2);

#endif
