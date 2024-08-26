#ifndef obj_set_h
#define obj_set_h

#include "obj.h"

#define OBJ_SET 1024

struct obj_set_t {
  obj_t object[OBJ_SET];
  long size;
  long it;
  enum obj_bool_t itremove;
};

void obj_set_add(struct obj_set_t *set, obj_t obj);
void obj_set_remove(struct obj_set_t *set, obj_t obj);
enum obj_bool_t obj_set_find(struct obj_set_t *set, obj_t obj);

void obj_set_itstart(struct obj_set_t *set);
obj_t obj_set_itnext(struct obj_set_t *set);
void obj_set_itremove(struct obj_set_t *set);

void obj_set_limitsize(struct obj_set_t *set, long size);

void obj_set_print(struct obj_set_t *set);

#endif
