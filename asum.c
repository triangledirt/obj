#include <stdio.h>
#include "abit.h"
#include "alib.h"
#include "aobj.h"
#include "asum.h"

static aobj_t ideal[ALIB_TYPE_COUNT];

abit_t asum_classify(aobj_t obj, long type)
{
  return (aobj_compare(obj, ideal[type]) > 0.75);
}

void asum_learn(aobj_t objs[], long objs_size, long type)
{
  long idx;
  long bit;
  abit_t val;
  aobj_t obj;
  long onecounts[32];
  long thresh = objs_size / 4;
  abit_t class;
  for (bit = 1; bit < 32; bit++) {
    onecounts[bit] = 0;
  }
  for (idx = 0; idx < objs_size; idx++) {
    obj = objs[idx];
    class = aobj_getclass(obj);
    if (class) {
      for (bit = 1; bit < 32; bit++) {
        val = aobj_getattr(obj, bit);
        if (val) {
          onecounts[bit]++;
        }
      }
    }
  }
  for (bit = 1; bit < 32; bit++) {
    if (onecounts[bit] > thresh) {
      aobj_setattr(&ideal[type], bit, 1);
    } else {
      aobj_setattr(&ideal[type], bit, 0);
    }
  }
#if ALIB_VERBOSE
  printf("type%ld ideal sum ", type);
  aobj_print(ideal[type]);
  printf("\n");
#endif
}
