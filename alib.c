#include <stdio.h>
#include <stdlib.h>
#include "abit.h"
#include "alib.h"
#include "aobj.h"
#include "asum.h"

#define OBJECT_CACHE 64

static char initd = 0;
static aobj_t objs[ALIB_TYPE_COUNT][OBJECT_CACHE];
static long types = 1;

static void init();
static void learn();
static void uptypes(long seentype);

abit_t alib_classify(aobj_t obj, long type) {
  init();
  uptypes(type);
  return asum_classify(obj, type);
  return 0;
}

void alib_observe(aobj_t obj, long type) {
  long idx;
  init();
  uptypes(type);
  idx = random() % OBJECT_CACHE;
  objs[type][idx] = obj;
  if (0 == (random() % (OBJECT_CACHE / 16))) {
    learn();
  }
}

void learn() {
  long type;
  for (type = 0; type < types; type++) {
    asum_learn(objs[type], OBJECT_CACHE, type);
  }
}

void init() {
  long idx;
  long type;
  if (!initd) {
    for (type = 0; type < ALIB_TYPE_COUNT; type++) {
      for (idx = 0; idx < OBJECT_CACHE; idx++) {
        aobj_randomize(&objs[type][idx]);
      }
    }
    initd = 1;
  }
}

void uptypes(long seentype) {
  if ((seentype + 1) > types) {
    types = seentype;
  }
}
