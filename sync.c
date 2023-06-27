#include <stdlib.h>
#include "sync.h"
#include "syncstyle.h"

static obj_t object[OBJ_TYPE][OBJ_SYNC_CACHE];
static enum obj_syncstyle_t syncstyle[OBJ_TYPE];
static obj_bool_t valid[OBJ_TYPE];
static obj_bool_t once = obj_bool_false;

static void init();
static void sync(long type);
static void sync_resource(long type);
static void sync_person(long type);

void init()
{
  long i;
  long j;
  obj_t obj;
  if (!once) {
    for (i = 0; i < OBJ_TYPE; i++) {
      valid[i] = obj_bool_true;
      syncstyle[i] = obj_syncstyle_resource;
      for (j = 0; j < OBJ_SYNC_CACHE; j++) {
        obj_randomize(&obj);
        object[i][j] = obj;
      }
    }
    once = obj_bool_true;
  }
}

void obj_sync_add(obj_t obj, long type)
{
  long i;
  init();
  i = random() % OBJ_SYNC_CACHE;
  object[type][i] = obj;
  valid[type] = obj_bool_false;
}

obj_bool_t obj_sync_next(obj_t *next, long set, long type)
{
  init();
  ;;
}

void obj_sync_remove(obj_t obj, long type)
{
  long i;
  init();
  for (i = 0; i < OBJ_SYNC_CACHE; i++) {
    if (object[type][i] == obj) {
      obj_randomize(&object[type][i]);
      valid[type] = obj_bool_false;
      break;
    }
  }
}

void obj_sync_setstyle(enum obj_syncstyle_t style, long type)
{
  init();
  syncstyle[type] = style;
  valid[type] = obj_bool_false;
}

void obj_sync_start(long set, long type)
{
  init();
  sync(type);
  ;;
}

void sync(long type)
{
  if (!valid[type]) {
    switch (syncstyle[type]) {
    case obj_syncstyle_person:
      sync_person(type);
      break;
    case obj_syncstyle_resource:
      sync_resource(type);
      break;
    }
    valid[type] = obj_bool_true;
  }
}

void sync_resource(long type)
{
  ;;
}

void sync_person(long type)
{
  ;;
}
