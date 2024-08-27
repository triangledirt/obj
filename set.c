#include <stdio.h>
#include <stdlib.h>
#include "set.h"

static enum obj_bool_t once = obj_bool_false;

static void init(struct obj_set_t *set);

void init(struct obj_set_t *set)
{
  long object;
  if (!once) {
    for (object = 0; object < OBJ_SET; object++)
      obj_clear(&set->object[object]);
    set->size = OBJ_SET;
    once = obj_bool_true;
  }
}

void obj_set_add(struct obj_set_t *set, obj_t obj)
{
  long i;
  init(set);
  i = obj % set->size;
  set->object[i] = obj;
}

enum obj_bool_t obj_set_find(struct obj_set_t *set, obj_t obj)
{
  long i;
  init(set);
  i = obj % set->size;
  return (obj == set->object[i]) ? obj_bool_true : obj_bool_false;
}

void obj_set_itstart(struct obj_set_t *set)
{
  init(set);
  set->itremove = obj_bool_false;
  set->it = 0;
}

obj_t obj_set_itnext(struct obj_set_t *set)
{
  obj_t obj;
  if (set->itremove) {
    obj_clear(&set->object[set->it]);
    set->itremove = obj_bool_false;
  }
  obj = set->object[set->it];
  while (!obj && (set->it < (set->size - 1))) {
    if (set->it < (set->size - 1)) {
      set->it++;
      obj = set->object[set->it];
    } else {
      obj_clear(&obj);
      break;
    }
  }
  set->it++;
  return obj;
}

void obj_set_itremove(struct obj_set_t *set)
{
  set->itremove = obj_bool_true;
}

void obj_set_limitsize(struct obj_set_t *set, long size)
{
  set->size = size;
}

void obj_set_print(struct obj_set_t *set)
{
  obj_t obj;
  obj_set_itstart(set);
  while ((obj = obj_set_itnext(set))) {
    obj_print(obj);
    printf("\n");
  }
}

void obj_set_remove(struct obj_set_t *set, obj_t obj)
{
  long i;
  init(set);
  i = obj % set->size;
  if (obj == set->object[i]) {
    obj_clear(&set->object[i]);
  }
}
