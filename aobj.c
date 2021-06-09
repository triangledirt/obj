#include <stdlib.h>
#include "aobj.h"

void aobj_init(aobj_t *obj) {
  *obj = 0;
}

void aobj_randomize(aobj_t *obj) {
  *obj = random();
}
