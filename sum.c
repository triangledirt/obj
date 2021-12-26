#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "obj.h"
#include "sum.h"
#include "toss.h"

static double fitness = 0.0;
static case_obj_t ideal[32];

case_bit_t sum_classify(case_obj_t obj, long type)
{
  return case_obj_comparet(obj, ideal[type]) > (0.9 * fitness);
}

void sum_learn(case_obj_t objs[], long objssz, long type)
{
  long idx;
  long bit;
  case_bit_t val;
  case_obj_t obj;
  long onecounts[32];
  long thresh = objssz / 8;
  case_bit_t class;
#if CASE_VERBOSE
  double tot = 0;
#endif
  for (bit = 1; bit < 32; bit++)
    onecounts[bit] = 0;
  for (idx = 0; idx < objssz; idx++) {
    if (toss_coin())
      continue;
    obj = objs[idx];
    class = case_obj_getclass(obj);
    if (class)
      for (bit = 1; bit < 32; bit++) {
        val = case_obj_getattr(obj, bit);
        if (val)
          onecounts[bit]++;
      }
  }
  for (bit = 1; bit < 32; bit++)
    if (onecounts[bit] > thresh) {
      case_obj_setattr(&ideal[type], bit, 1);
    } else {
      case_obj_setattr(&ideal[type], bit, 0);
    }
#if CASE_VERBOSE
  for (idx = 0; idx < objssz; idx++)
    tot += case_obj_comparet(ideal[type], objs[idx]);
  fitness = tot / (objssz / 2);
  printf("type%ld ideal sum ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}
