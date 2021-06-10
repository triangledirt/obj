#include <stdio.h>
#include "abit.h"
#include "aideal.h"
#include "alib.h"
#include "aobj.h"

static aobj_t ideal[ALIB_TYPE_COUNT];

abit_t aideal_classify(aobj_t obj, long type) {
  long bit;
  long correct = 0;
  abit_t objbit;
  abit_t idealbit;
  abit_t class;
  for (bit = 1; bit < 32; bit++) {
    objbit = aobj_getattr(obj, bit);
    idealbit = aobj_getattr(ideal[type], bit);
    if (objbit == idealbit) {
      correct++;
    }
  }
  if ((correct / 31) > 0.75) {
    class = 1;
  } else {
    class = 0;
  }
  return class;
}

void aideal_learn(aobj_t objs[], long objs_size, long type) {
  long idx;
  long bit;
  abit_t bitvalue;
  aobj_t obj;
  long onecounts[32];
  long thresh = objs_size / 2;
  for (bit = 0; bit < 32; bit++) {
    onecounts[bit] = 0;
  }
  for (idx = 0; idx < objs_size; idx++) {
    obj = objs[idx];
    for (bit = 0; bit < 32; bit++) {
      bitvalue = aobj_getattr(obj, bit);
      if (bitvalue) {
        onecounts[bit]++;
      }
    }
  }
  for (bit = 0; bit < 32; bit++) {
    if (onecounts[bit] > thresh) {
      aobj_setattr(&ideal[type], bit, 1);
    } else {
      aobj_setattr(&ideal[type], bit, 0);
    }
  }
  printf("ideal::");
  aobj_print(ideal[type]);
}
