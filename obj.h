#ifndef obj_h
#define obj_h

#include "bool.h"

#define CASE_OBJ 64
typedef long long case_obj_t;
#define CASE_OBJ_CLEAR 0

void case_obj_clear(case_obj_t *obj);
void case_obj_fill(case_obj_t *obj);
void case_obj_randomize(case_obj_t *obj);
void case_obj_setfromstr(case_obj_t *obj, char str[CASE_OBJ]);

#define case_obj_setattr(obj, indx, val) if (val) { *obj |= ((long long) 1 << indx); } else { *obj &= ~((long long) 1 << indx); }
#define case_obj_getattr(obj, indx) ((obj >> indx) & (long long) 1)
#define case_obj_setclass(obj, val) if (val) { *obj |= ((long long) 1); } else { *obj &= ~((long long) 1); }
#define case_obj_getclass(obj) (obj & (long long) 1)

double case_obj_comparebox(case_obj_t obj1, case_obj_t obj2);
double case_obj_compareequal(case_obj_t obj1, case_obj_t obj2);
double case_obj_comparefocus(case_obj_t obj1, case_obj_t obj2);
double case_obj_compareoblivion(case_obj_t obj1, case_obj_t obj2);
double case_obj_comparetypes(case_obj_t obj1, case_obj_t obj2);
double case_obj_comparexor(case_obj_t obj1, case_obj_t obj2);
#define case_obj_compare(obj1, obj2) case_obj_comparetypes(obj1, obj2)

case_bool_t case_obj_hastype(case_obj_t obj, case_obj_t type);
void case_obj_print(case_obj_t obj);

void case_obj_obscureclass(case_obj_t *obj);
void case_obj_mutate(case_obj_t *obj);
void case_obj_rotate(case_obj_t *obj, long inc);

void case_obj_setnum(case_obj_t *obj, long startbit, long bits, long long num);
long long case_obj_getnum(case_obj_t obj, long startbit, long bits);

long case_obj_smash(case_obj_t obj1, case_obj_t obj2);

#endif
