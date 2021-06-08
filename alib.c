#include <stdlib.h>
#include "abit.h"
#include "aobj.h"

#define OBJECT_CACHE 1024

static char initialized = 0;
static aobj_t objs[OBJECT_CACHE];

static void alib_init();

void alib_init() {
  if (!initialized) {
    long idx;
    for (idx = 0; idx < OBJECT_CACHE; idx++) {
      objs[idx] = 0;
    }
  } else {
    initialized = 1;
  }
}

void alib_notice(aobj_t obj) {
  alib_init();
  long idx = random() % OBJECT_CACHE;
  objs[idx] = obj;
}

abit_t alib_classify(aobj_t obj) {
  alib_init();
  return 0;
}
