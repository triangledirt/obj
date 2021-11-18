#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "object.h"
#include "sum.h"

static double fitness = 0.0;
static case_object_t ideal[32];

case_bit_t sum_classify(case_object_t obj, long type)
{
  return case_object_comparet(obj, ideal[type]) > (0.9 * fitness);
}

void sum_learn(case_object_t objs[], long objs_size, long type)
{
  long idx;
  long bit;
  case_bit_t val;
  case_object_t obj;
  long onecounts[32];
  long thresh = objs_size / 8;
  case_bit_t class;
#if CASE_VERBOSE
  double tot = 0;
#endif
  for (bit = 1; bit < 32; bit++) {
    onecounts[bit] = 0;
  }
  for (idx = 0; idx < objs_size; idx++) {
    obj = objs[idx];
    class = case_object_getclass(obj);
    if (class) {
      for (bit = 1; bit < 32; bit++) {
        val = case_object_getattr(obj, bit);
        if (val) {
          onecounts[bit]++;
        }
      }
    }
  }
  for (bit = 1; bit < 32; bit++) {
    if (onecounts[bit] > thresh) {
      case_object_setattr(&ideal[type], bit, 1);
    } else {
      case_object_setattr(&ideal[type], bit, 0);
    }
  }
#if CASE_VERBOSE
  for (idx = 0; idx < objs_size; idx++) {
    tot += case_object_comparet(ideal[type], objs[idx]);
  }
  fitness = tot / objs_size;
  printf("type%ld ideal sum ", type);
  case_object_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}
