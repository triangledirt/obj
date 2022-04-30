#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "die.h"
#include "moire.h"
#include "obj.h"
#include "xbit.h"

static xbit_t past[CASE_TYPE];
static case_bool_t once = case_bool_false;

static void init();

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < CASE_TYPE; type++)
      xbit_init(&past[type]);
    once = case_bool_true;
  }
}

void moire_learn(case_obj_t obj[], long objsz, long type)
{
  long i;
  double onepart;
  init();
  for (i = 0; i < objsz; i++)
    if (die_toss(XBIT_CNT))
      xbit_note(&past[type], case_obj_getclass(obj[i]));
#if CASE_VERBOSE
  onepart = 1 - xbit_zeropart(&past[type]);
  printf("type%02ld onept moi ", type);
  printf("                                                                ");
  printf(" %0.3f\n", onepart);
#endif
}

double moire_score(case_obj_t obj, long type)
{
  init();
  return 1 - xbit_zeropart(&past[type]);
}
