#include <stdlib.h>
#include "set.h"

static enum obj_bool_t once = obj_bool_false;

static long findinbucket(struct obj_set_t *set, long bucket, obj_t obj);
static enum obj_bool_t forwardnextobject(struct obj_set_t *set);
static void init(struct obj_set_t *set);
static void pack(struct obj_set_t *set, long bucket, long startobject);
static void removeindex(struct obj_set_t *set, long bucket, long object);
static void removeobj(struct obj_set_t *set, long bucket, obj_t obj);

long findinbucket(struct obj_set_t *set, long bucket, obj_t obj)
{
  long object;
  obj_t xobj;
  long foundindex = -1;
  init(set);
  bucket = obj % OBJ_SET_BUCKET;
  for (object = 0; object < OBJ_SET_BUCKETSZ; object++) {
    xobj = set->bucket[bucket][object];
    if (xobj == obj) {
      foundindex = object;
      break;
    }
  }
  return foundindex;
}

enum obj_bool_t forwardnextobject(struct obj_set_t *set)
{
  enum obj_bool_t found = obj_bool_false;
  while (!found && (set->itbucket < OBJ_SET_BUCKET)) {
    if (set->itobject < (OBJ_SET_BUCKETSZ - 1)) {
      set->itobject++;
      if (set->bucket[set->itbucket][set->itobject]) {
        found = obj_bool_true;
      }
    } else {
      if (set->itbucket < (OBJ_SET_BUCKET - 1)) {
        set->itbucket++;
        set->itobject = 0;
      }
    }
  }
  return found;
}

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
  if (set->itremove)
    removeindex(set, set->itbucket, set->itobject);
  if (forwardnextobject(set)) {
    obj = set->bucket[set->itbucket][set->itobject];
  } else {
    obj_clear(&obj);
  }
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
  init(set);
  bucket = obj % OBJ_SET_BUCKET;
  removeobj(set, bucket, obj);
}

void pack(struct obj_set_t *set, long bucket, long startobject)
{
  long object;
  for (object = startobject; object < (OBJ_SET_BUCKETSZ - 1); object++)
    set->bucket[bucket][object] = set->bucket[bucket][object + 1];
  obj_clear(&set->bucket[bucket][OBJ_SET_BUCKETSZ - 1]);
}

void removeindex(struct obj_set_t *set, long bucket, long object)
{
  pack(set, bucket, object);
}

void removeobj(struct obj_set_t *set, long bucket, obj_t obj)
{
  long object;
  object = findinbucket(set, bucket, obj);
  if (object != -1)
    pack(set, bucket, object);
}