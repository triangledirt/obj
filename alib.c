#include <stdlib.h>
#include "abit.h"
#include "aobj.h"

#define OBJECT_CACHE 1024
#define LEARN_FREQ 32

static char initialized = 0;
static aobj_t objs[OBJECT_CACHE];

static void alib_init();
static void alib_learn();

void alib_init() {
  if (!initialized) {
    long idx;
    for (idx = 0; idx < OBJECT_CACHE; idx++) {
      objs[idx] = random();
    }
  } else {
    initialized = 1;
  }
}

void alib_learn() {}

void alib_notice(aobj_t obj) {
  alib_init();
  long idx = random() % OBJECT_CACHE;
  objs[idx] = obj;
  if (0 == (random() % LEARN_FREQ)) {
    alib_learn();
  }
}

abit_t alib_classify(aobj_t obj) {
  alib_init();
  return 0;
}
