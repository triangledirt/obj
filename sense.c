#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bit.h"
#include "sense.h"

#define CACHE OBJ
#define THRESH 0.5

obj_t cache[OBJ_TYPE][CACHE];
enum obj_bool_t once = obj_bool_false;

static obj_t buildattrobj(long indx, long type);
static void init();

obj_t buildattrobj(long indx, long type)
{
  long i;
  obj_t attrobj;
  obj_t obj;
  obj_bit_t bit;
  for (i = 0; i < CACHE; i++) {
    obj = cache[type][i];
    bit = obj_getattr(obj, indx);
    obj_setattr(&attrobj, i, bit);
  }
  return attrobj;
}

void init()
{
  long type;
  long i;
  if (!once) {
    srandom(time(NULL));
    for (type = 0; type < OBJ_TYPE; type++)
      for (i = 0; i < CACHE; i++)
        obj_randomize(&cache[type][i]);
    once = obj_bool_true;
  }
}

obj_t obj_sense_alive(long type)
{
  obj_t alive;
  obj_t classobj;
  obj_t attrobj;
  long i;
  double focus;
  init();
  classobj = buildattrobj(0, type);
  obj_clear(&alive);
  for (i = 1; i < CACHE; i++) {
    attrobj = buildattrobj(i, type);
    focus = obj_comparefocus(classobj, attrobj);
    if (focus > THRESH)
      obj_setattr(&alive, i, 1);
  }
  return alive;
}

void obj_sense_observe(obj_t obj, long type)
{
  long i;
  i = random() % CACHE;
  cache[type][i] = obj;
}
