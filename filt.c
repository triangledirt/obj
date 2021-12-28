#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "filt.h"
#include "obj.h"
#include "toss.h"

#define ACTS 8

/* bitmasks saying which of the bits in the object-to-be-classified get points for being either a one or a zero */
static case_obj_t one;
static case_obj_t zero;
static case_bit_t once = 0;

static void initonce();

case_bit_t filt_classify(case_obj_t obj, long type)
{
  case_bit_t class;
  long onetot = 0;
  long zerotot = 0;
  long onematch = 0;
  long zeromatch = 0;
  long bit;
  initonce();
  for (bit = 1; bit < 32; bit++) {
    onetot += case_obj_getattr(one, bit);
    zerotot += case_obj_getattr(zero, bit);
  }
  for (bit = 1; bit < 32; bit++) {
    if (case_obj_getattr(one, bit) && case_obj_getattr(obj, bit))
      onematch++;
    if (case_obj_getattr(zero, bit) && !case_obj_getattr(obj, bit))
      zeromatch++;
  }
  class = (((onematch + zeromatch) / (onetot + zerotot)) > 0.9) ? 1 : 0;
  return class;
}

void filt_learn(case_obj_t obj[], long objsz, long type)
{
  long idx;
  long bit;
  case_obj_t o;
  long act;
#if CASE_VERBOSE
  double tot = 0;
#endif
  initonce();
  for (act = 0; act < ACTS; act++) {
    ;; save the current one and zero off to the side
    ;; mutate both the one and zero objects
    ;; calculate ..
  }

  for (idx = 0; idx < objsz; idx++) {
    if (toss_coin())
      continue;
    o = obj[idx];
    ;;
    mutate(&one);
    ;;
  }

#if CASE_VERBOSE
  for (idx = 0; idx < objsz; idx++)
    tot += case_obj_comparet(ideal[type], obj[idx]);
  fitness = tot / (objsz / 2);
  printf("type%ld ideal filt ", type);
  case_obj_print(ideal[type]);
  printf(" %0.3f%%\n", fitness);
#endif
}

void initonce()
{
  if (!once) {
    case_obj_randomize(&one);
    case_obj_randomize(&zero);
    once = 1;
  }
}
