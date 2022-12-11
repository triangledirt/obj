#include <stdio.h>
#include "bit.h"
#include "class.h"
#include "coin.h"
#include "filt.h"
#include "obj.h"

static obj_bool_t once = obj_bool_false;
static obj_t one[OBJ_TYPE];
static obj_t onesv[OBJ_TYPE];
static double fitness[OBJ_TYPE];
static double fitnesssv[OBJ_TYPE];
static obj_t zero[OBJ_TYPE];
static obj_t zerosv[OBJ_TYPE];

static void calcfit(obj_t obj[], long objsz, long type);
static void init();
static void mutate(obj_t *obj1, obj_t *obj2);
static void restore(long type);
static void save(long type);
static double score(obj_t obj, long type);

void calcfit(obj_t obj[], long objsz, long type)
{
  long i;
  double s = 0.0;
  for (i = 0; i < objsz; i++)
    if (obj_coin_toss())
      s += score(obj[i], type);
  fitness[type] = s / (objsz / 2);
}

double obj_filt_fit(long type)
{
  return fitness[type];
}

void obj_filt_learn(obj_t obj[], long objsz, long type)
{
  long bit;
  obj_t o;
  long time;
  init();
  for (time = 0; time < 4; time++) {
    calcfit(obj, objsz, type);
    save(type);
    mutate(&one[type], &zero[type]);
    calcfit(obj, objsz, type);
    if (fitnesssv[type] > fitness[type])
      restore(type);
  }
#if OBJ_VERBOSE
  printf("type%02ld filtr ft1 ", type);
  obj_print(one[type]);
  printf(" %0.3f\ntype%02ld filtr ft0 ", fitness[type], type);
  obj_print(zero[type]);
  printf(" %0.3f\n", fitness[type]);
#endif
}

double obj_filt_score(obj_t obj, long type)
{
  init();
  return score(obj, type);
}

void init()
{
  long type;
  if (!once) {
    for (type = 0; type < OBJ_TYPE; type++) {
      obj_clear(&one[type]);
      obj_clear(&zero[type]);
    }
    once = obj_bool_true;
  }
}

void mutate(obj_t *obj1, obj_t *obj2)
{
  obj_t *o1;
  obj_t *o2;
  long bit;
  obj_bit_t val;
  long time;
  for (time = 0; time < 4; time++) {
    bit = random() % OBJ;
    obj_bit_randomize(&val);
    if (obj_coin_toss()) {
      o1 = obj1;
      o2 = obj2;
    } else {
      o1 = obj2;
      o2 = obj1;
    }
    obj_setattr(o1, bit, val);
    obj_setattr(o2, bit, 0);
  }
}

void restore(long type)
{
  one[type] = onesv[type];
  zero[type] = zerosv[type];
  fitness[type] = fitnesssv[type];
}

void save(long type)
{
  onesv[type] = one[type];
  zerosv[type] = zero[type];
  fitnesssv[type] = fitness[type];
}

double score(obj_t obj, long type)
{
  long bit;
  long onetot = 0;
  long zerotot = 0;
  long onematch = 0;
  long zeromatch = 0;
  obj_bit_t onebit;
  obj_bit_t zerobit;
  obj_bit_t objbit;
  long edge;
  edge = obj_edge(obj, OBJ_CLEAR);
  for (bit = 1; bit <= edge; bit++) {
    onebit = obj_getattr(one[type], bit);
    zerobit = obj_getattr(zero[type], bit);
    onetot += onebit;
    zerotot += zerobit;
    objbit = obj_getattr(obj, bit);
    if (onebit && objbit)
      onematch++;
    if (zerobit && !objbit)
      zeromatch++;
  }
  return (double) (onematch + zeromatch) / (1 + onetot + zerotot);
}
