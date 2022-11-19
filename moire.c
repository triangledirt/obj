#include <stdio.h>
#include "bit.h"
#include "class.h"
#include "die.h"
#include "moire.h"
#include "obj.h"
#include "xbit.h"

static obj_xbit_t past[OBJ_CLASS_TYPE];
static obj_bool_t once = obj_bool_false;

static void init();

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < OBJ_CLASS_TYPE; type++)
      obj_xbit_init(&past[type]);
    once = obj_bool_true;
  }
}

void obj_moire_learn(obj_t obj[], long objsz, long type)
{
  long i;
  double zeropart;
  init();
  for (i = 0; i < objsz; i++)
    if (obj_die_toss(OBJ_XBIT_CNT))
      obj_xbit_note(&past[type], obj_getclass(obj[i]));
#if OBJ_VERBOSE
  zeropart = obj_xbit_zeropart(&past[type]);
  printf("type%02ld zerop moi ", type);
  printf("                                                                ");
  printf(" %0.3f\n", zeropart);
#endif
}

double obj_moire_score(obj_t obj, long type)
{
  init();
  return 1 - obj_xbit_zeropart(&past[type]);
}
