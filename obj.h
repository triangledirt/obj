#ifndef obj_h
#define obj_h

#include "bool.h"
#include "game1.h"
#include "game2.h"
#include "game3.h"
#include "stat.h"

#define OBJ_VERBOSE 1
#define OBJ_XVERBOSE 1

#define OBJ 64
typedef long long obj_t;
#define OBJ_CLEAR 0
#define OBJ_CSV 4096
#define OBJ_TYPE OBJ

#define OBJ_SINGLE (OBJ / 2)

void obj_clear(obj_t *obj);
void obj_fill(obj_t *obj);
void obj_randomize(obj_t *obj);
void obj_setfromstr(obj_t *obj, char str[OBJ]);
void obj_singlize(obj_t *obj);

#define obj_setattr(obj, index, val) if (val) { *obj |= ((long long) 1 << index); } else { *obj &= ~((long long) 1 << index); }
#define obj_attr(obj, index) ((obj >> index) & (long long) 1)
void obj_flipattr(obj_t *obj, long index);
void obj_setnum(obj_t *obj, long start, long length, long long num);
long long obj_num(obj_t obj, long start, long length);

#define obj_setclass(obj, val) if (val) { *obj |= ((long long) 1); } else { *obj &= ~((long long) 1); }
#define obj_class(obj) (obj & (long long) 1)

#define obj_setsingle(obj, val) if (val) { *obj |= ((long long) 1 << OBJ_SINGLE); } else { *obj &= ~((long long) 1 << OBJ_SINGLE); }
#define obj_single(obj) ((obj >> OBJ_SINGLE) & (long long) 1)

long obj_edge(obj_t obj1, obj_t obj2);
enum obj_bool_t obj_hastype(obj_t obj, obj_t type);
void obj_print(obj_t obj);
#define obj_randomindex() (random() % OBJ)
struct obj_stat_t obj_stat(obj_t obj);

double obj_comparebox(obj_t obj1, obj_t obj2);
double obj_compareequal(obj_t obj1, obj_t obj2);
double obj_comparefocus(obj_t obj1, obj_t obj2);
double obj_compareoblivion(obj_t obj1, obj_t obj2);
double obj_comparesquare(obj_t obj1, obj_t obj2);
double obj_comparetypes(obj_t obj1, obj_t obj2);
double obj_comparexor(obj_t obj1, obj_t obj2);
#define obj_compare(obj1, obj2) obj_comparesquare(obj1, obj2)

void obj_morph1(obj_t *obj, obj_game1_t game1, long ticks);
void obj_morph2(obj_t *obj, obj_game2_t game2, long ticks);
void obj_morph3(obj_t *obj, obj_game3_t game3, long ticks);

void obj_obscureclass(obj_t *obj);
void obj_mutate(obj_t *obj);
void obj_rotate(obj_t *obj, long inc);

typedef double (*obj_fit_f)(obj_t obj, long type, void *context);

#endif
