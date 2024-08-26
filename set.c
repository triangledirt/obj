#include <stdlib.h>
#include "set.h"

static enum obj_bool_t once = obj_bool_false;

static void init(struct obj_set_t *set);

void init(struct obj_set_t *set)
{
  long bucket;
  long object;
  if (!once) {
    for (bucket = 0; bucket < OBJ_SET_BUCKET; bucket++)
      for (object = 0; object < OBJ_SET_BUCKETSZ; object++)
        obj_clear(&set->bucket[bucket][object]);
    once = obj_bool_true;
  }
}

void obj_set_add(struct obj_set_t *set, obj_t obj)
{
  long bucket;
  long object = 0;
  obj_t xobj;
  enum obj_bool_t added = obj_bool_false;
  init(set);
  bucket = obj % OBJ_SET_BUCKET;
  for (object = 0; object < OBJ_SET_BUCKETSZ; object++) {
    xobj = set->bucket[bucket][object];
    if (0 == xobj) {
      set->bucket[bucket][object] = obj;
      added = obj_bool_true;
      break;
    } else if (obj == xobj) {
      added = obj_bool_true;
      break;
    }
  }
  if (!added) {
    object = random() % OBJ_SET_BUCKETSZ;
    set->bucket[bucket][object] = obj;
  }
}

enum obj_bool_t obj_set_find(struct obj_set_t *set, obj_t obj)
{
  long bucket;
  long object = 0;
  obj_t xobj;
  enum obj_bool_t found = obj_bool_false;
  init(set);
  bucket = obj % OBJ_SET_BUCKET;
  for (object = 0; object < OBJ_SET_BUCKETSZ; object++) {
    xobj = set->bucket[bucket][object];
    if (xobj == obj) {
      found = obj_bool_true;
      break;
    }
  }
  return found;
}

void obj_set_itstart(struct obj_set_t *set)
{
  init(set);
  set->itbucket = 0;
  set->itobject = 0;
  set->itremove = obj_bool_false;
}

obj_t obj_set_itnext(struct obj_set_t *set)
{
  obj_t obj;
  init(set);
  if (set->bucket[set->itbucket][set->itobject]) {
    obj = set->bucket[set->itbucket][set->itobject];
  } else {
  }
  /* ff to next nonzero objectn */
  return obj;
}

void obj_set_itremove(struct obj_set_t *set)
{
  init(set);
  set->itremove = obj_bool_true;
}

void obj_set_remove(struct obj_set_t *set, obj_t obj)
{
  long bucket;
  long object = 0;
  obj_t xobj;
  enum obj_bool_t found = obj_bool_false;
  init(set);
  bucket = obj % OBJ_SET_BUCKET;
  for (object = 0; object < OBJ_SET_BUCKETSZ; object++) {
    xobj = set->bucket[bucket][object];
    if (xobj == obj) {
      found = obj_bool_true;
      break;
    }
  }
  if (found) {
    for (object = object; object < (OBJ_SET_BUCKETSZ - 1); object++)
      set->bucket[bucket][object] = set->bucket[bucket][object + 1];
    obj_clear(&set->bucket[bucket][OBJ_SET_BUCKETSZ - 1]);
  }
}