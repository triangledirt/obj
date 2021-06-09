#include <stdlib.h>
#include "abit.h"
#include "aideal.h"
#include "alib.h"
#include "aobj.h"

#define OBJECT_CACHE 1024

static char initd = 0;
static aobj_t objs[ALIB_TYPE_COUNT][OBJECT_CACHE];
static long types = 1;

static void alib_init();
static void alib_learn();
static void alib_uptypes(long seentype);

abit_t alib_classify(aobj_t obj, long type) {
  alib_init();
  alib_uptypes(type);
  return aideal_classify(obj, type);
  return 0;
}

void alib_init() {
  long idx;
  long type;
  if (!initd) {
    for (type = 0; type < ALIB_TYPE_COUNT; type++) {
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
  for (type = 0; type < ALIB_TYPE_COUNT; type++) {
    aideal_learn(objs[type], OBJECT_CACHE, type);
  }
}

void alib_observe(aobj_t obj, long type) {
  long idx;
  alib_init();
  alib_uptypes(type);
  idx = random() % OBJECT_CACHE;
  objs[type][idx] = obj;
  if (0 == (random() % (OBJECT_CACHE / 32))) {
    alib_learn();
  }
}

void alib_uptypes(long seentype) {
  if ((seentype + 1) > types) {
    types = seentype;
  }
}
