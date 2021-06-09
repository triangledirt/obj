#include <stdlib.h>
#include "abit.h"
#include "alib.h"
#include "aobj.h"
#include "apole.h"

#define OBJECT_CACHE 1024
#define TYPES_MAX 8

static char initd = 0;
static aobj_t objs[TYPES_MAX][OBJECT_CACHE];
static long types = 1;

static void alib_init();
static void alib_learn();
static void alib_uptypes(long seentype);

void alib_init() {
  long idx;
  long type;
  if (!initd) {
    for (type = 0; type < TYPES_MAX; type++) {
      for (idx = 0; idx < OBJECT_CACHE; idx++) {
        objs[type][idx] = random();
      }
    }
  } else {
    initd = 1;
  }
}

void alib_learn() {
  long type;
  for (type = 0; type < TYPES_MAX; type++) {
    apole_learn(objs[type], OBJECT_CACHE, type);
  }
}

void alib_notice(aobj_t obj, long type) {
  long idx;
  alib_init();
  alib_uptypes(type);
  idx = random() % OBJECT_CACHE;
  objs[type][idx] = obj;
  if (0 == (random() % (OBJECT_CACHE / 32))) {
    alib_learn();
  }
}

abit_t alib_classify(aobj_t obj, long type) {
  alib_init();
  alib_uptypes(type);
  return apole_classify(obj);
  return 0;
}

void alib_uptypes(long seentype) {
  if ((seentype + 1) > types) {
    types = seentype;
  }
}
