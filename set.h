#ifndef obj_set_h
#define obj_set_h

#include "obj.h"

#define OBJ_SET_BUCKET 256
#define OBJ_SET_BUCKETSZ 8

struct obj_set_t {
  obj_t bucket[OBJ_SET_BUCKET][OBJ_SET_BUCKETSZ];
  long itbucket;
  long itobject;
  enum obj_bool_t itremove;
};

void obj_set_add(struct obj_set_t *set, obj_t obj);
void obj_set_remove(struct obj_set_t *set, obj_t obj);
enum obj_bool_t obj_set_find(struct obj_set_t *set, obj_t obj);

void obj_set_itstart(struct obj_set_t *set);
obj_t obj_set_itnext(struct obj_set_t *set);
void obj_set_itremove(struct obj_set_t *set);

#endif