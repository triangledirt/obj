#include <stdio.h>
#include <stdlib.h>
#include "abit.h"
#include "aobj.h"

void aobj_clear(aobj_t *obj) {
  *obj = 0;
}

double aobj_compare(aobj_t obj1, aobj_t obj2) {
  long bit;
  long correct = 0;
  abit_t bit1;
  abit_t bit2;
  for (bit = 1; bit < 31; bit++) {
    bit1 = aobj_getattr(obj1, bit);
    bit2 = aobj_getattr(obj2, bit);
    if (bit1 == bit2) {
      correct++;
    }
  }
  return (double) correct / 31;
}

void aobj_mutate(aobj_t *obj) {
  long idx = random() % 32;
  abit_t val = abit_random();
  aobj_setattr(obj, idx, val);
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
