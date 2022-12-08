#include "bit.h"
#include "sense.h"

#define OBSERVE 8

long changes[OBJ_TYPE][OBJ];
obj_t past[OBJ_TYPE];
long observecnt;
obj_bool_t once = obj_bool_false;

static void init();

void init()
{
  long type;
  long i;
  if (!once) {
    observecnt = 0;
    for (type = 0; type < OBJ_TYPE; type++) {
      for (i = 0; i < OBJ; i++)
        changes[type][i] = 0;
      obj_randomize(&past[type]);
    }
    once = obj_bool_true;
  }
}

obj_t obj_sense_alive(long type)
{
  obj_t alive;
  long i;
  long thresh;
  init();
  obj_clear(&alive);
  thresh = observecnt / 4;
  for (i = 0; i < OBJ; i++)
    if (changes[type][i] > thresh)
      obj_setattr(&alive, i, 1);
  if (observecnt >= OBSERVE) {
    once = obj_bool_false;
    init();
  }
  return alive;
}

/*
add up how many 1s in each column for that list and range those onto a 0.0–1.0 number for each column—and use that to return alive ??
*/

void obj_sense_observe(obj_t obj, long type)
{
  long i;
  obj_bit_t bit1;
  obj_bit_t bit2;
  init();
  for (i = 0; i < OBJ; i++) {
    bit1 = obj_getattr(past[type], i);
    bit2 = obj_getattr(obj, i);
    if (bit1 != bit2)
      changes[type][i]++;
  }
  past[type] = obj;
  observecnt++;
}
