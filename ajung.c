#include <stdio.h>
#include "abit.h"
#include "ajung.h"
#include "alib.h"
#include "aobj.h"

static aobj_t ideal[ALIB_TYPE_COUNT];

abit_t ajung_classify(aobj_t obj, long type)
{
  return (aobj_compare(obj, ideal[type]) > 0.75);
}

void ajung_learn(aobj_t objs[], long objs_size, long type)
{
#if ALIB_VERBOSE
  printf("type%ld ideal jng ", type);
  aobj_print(ideal[type]);
  printf("\n");
#endif
}
