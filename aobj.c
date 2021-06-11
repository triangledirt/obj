#include <stdio.h>
#include <stdlib.h>
#include "aobj.h"

void aobj_init(aobj_t *obj) {
  *obj = 0;
}

void aobj_print(aobj_t obj) {
  long idx;
  for (idx = 0; idx < 32; idx++) {
    printf("%lu", aobj_getattr(obj, idx));
  }
}

void aobj_randomize(aobj_t *obj) {
  *obj = random();
}
