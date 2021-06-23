#include <stdio.h>
#include <stdlib.h>
#include "abit.h"
#include "aobj.h"

void aobj_clear(aobj_t *obj)
{
  *obj = 0;
}

double aobj_compare(aobj_t obj1, aobj_t obj2)
{
  long bit;
  long correct = 0;
  abit_t bit1;
  abit_t bit2;
  for (bit = 1; bit < 31; bit++) {
    bit1 = aobj_getattr(obj1, bit);
    bit2 = aobj_getattr(obj2, bit);
    if (bit1 == bit2) {
      correct++;
    }
  }
  return (double) correct / 31;
}

long aobj_getnum(aobj_t *obj, long startbit, long bits)
{
  long place = 1;
  long bit;
  long num = 0;
  for (bit = startbit; bit < bits; bit++) {
    num += place * aobj_getattr(*obj, bit);
    place *= 2;
  }
  return num;
}

void aobj_mutate(aobj_t *obj)
{
  long idx = random() % 32;
  abit_t val = abit_random();
  aobj_setattr(obj, idx, val);
}

void aobj_print(aobj_t obj)
{
  long idx;
  for (idx = 0; idx < 32; idx++) {
    printf("%lu", aobj_getattr(obj, idx));
  }
}

void aobj_randomize(aobj_t *obj)
{
  *obj = random();
}

void aobj_setnum(aobj_t *obj, long startbit, long bits, long num)
{
  long place = 2;
  long bit = startbit;
  abit_t val;
  long rem = num;
  do {
    val = rem % place;
    aobj_setattr(obj, bit, val);
    rem -= val;
    place *= 2;
    bit++;
  } while (bit < bits);
}
