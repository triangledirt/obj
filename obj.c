#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "bool.h"
#include "indx.h"
#include "obj.h"

static long findsmashpoint(case_obj_t obj1, case_obj_t obj2);

void case_obj_clear(case_obj_t *obj)
{
  *obj = 0;
}

double case_obj_comparebox(case_obj_t obj1, case_obj_t obj2)
{
  long smash;
  long i;
  long j;
  case_bit_t o1bit;
  case_bit_t o2bit;
  long and = 0;
  smash = findsmashpoint(obj1, obj2);
  for (i = 0; i < smash; i++)
    for (j = 0; j < smash; j++) {
      o1bit = case_obj_getattr(obj1, i);
      o2bit = case_obj_getattr(obj2, j);
      if (o1bit && o2bit)
	and++;
    }
  return and / pow(smash, 2);
}

double case_obj_compareequal(case_obj_t obj1, case_obj_t obj2)
{
  long bit;
  long smash;
  long correct = 0;
  case_bit_t bit1;
  case_bit_t bit2;
  smash = findsmashpoint(obj1, obj2);
  for (bit = 1; bit < smash; bit++) {
    bit1 = case_obj_getattr(obj1, bit);
    bit2 = case_obj_getattr(obj2, bit);
    if (bit1 == bit2)
      correct++;
  }
  return (double) correct / 1 + smash;
}

double case_obj_comparefocus(case_obj_t obj1, case_obj_t obj2)
{
  return 1 - case_obj_compareoblivion(obj1, obj2);
}

double case_obj_compareoblivion(case_obj_t obj1, case_obj_t obj2)
{
  long match;
  long opposite = 0;
  long smash;
  long i;
  case_bit_t o1bit;
  case_bit_t o2bit;
  smash = findsmashpoint(obj1, obj2);
  for (i = 0; i < smash; i++) {
    o1bit = case_obj_getattr(obj1, i);
    o2bit = case_obj_getattr(obj2, i);
    if (o1bit != o2bit)
      opposite++;
  }
  match = smash - opposite;
  return (double) (smash - labs(match - opposite)) / smash;
}

double case_obj_comparetypes(case_obj_t obj1, case_obj_t obj2)
{
  long bit;
  long correct = 0;
  long total = 0;
  case_bit_t bit1;
  case_bit_t bit2;
  for (bit = 1; bit < CASE_OBJ; bit++) {
    bit1 = case_obj_getattr(obj1, bit);
    bit2 = case_obj_getattr(obj2, bit);
    if ((1 == bit1) || (1 == bit2)) {
      total++;
      if ((1 == bit1) && (1 == bit2))
        correct++;
    }
  }
  return (double) correct / 1 + total;
}

double case_obj_comparexor(case_obj_t obj1, case_obj_t obj2)
{
  long bit;
  long smash;
  long correct = 0;
  case_bit_t bit1;
  case_bit_t bit2;
  smash = findsmashpoint(obj1, obj2);
  for (bit = 1; bit < smash; bit++) {
    bit1 = case_obj_getattr(obj1, bit);
    bit2 = case_obj_getattr(obj2, bit);
    if (bit1 ^ bit2)
      correct++;
  }
  return (double) correct / 1 + smash;
}

long long case_obj_getnum(case_obj_t obj, long startbit, long bits)
{
  long place = 1;
  long bit;
  long long num = 0;
  for (bit = startbit; bit < bits; bit++) {
    num += place * case_obj_getattr(obj, bit);
    place *= 2;
  }
  return num;
}

case_bool_t case_obj_hastype(case_obj_t obj, case_obj_t type)
{
  case_bool_t has = case_bool_true;
  long i;
  for (i = 0; i < CASE_OBJ; i++)
    if (case_obj_getattr(type, i))
      if (!case_obj_getattr(obj, i)) {
        has = case_bool_false;
        break;
      }
  return has;
}

void case_obj_mutate(case_obj_t *obj)
{
  long i;
  case_bit_t val;
  i = random() % CASE_OBJ;
  case_bit_randomize(&val);
  case_obj_setattr(obj, i, val);
}

void case_obj_obscureclass(case_obj_t *obj)
{
  case_obj_setclass(obj, case_bit_random());
}

void case_obj_print(case_obj_t obj)
{
  long i;
  for (i = 0; i < CASE_OBJ; i++)
    printf("%ld", case_obj_getattr(obj, i));
}

void case_obj_randomize(case_obj_t *obj)
{
  *obj = random();
}

void case_obj_rotate(case_obj_t *obj, long inc)
{
  case_obj_t obj2;
  long i;
  long newi;
  case_bit_t bit;
  for (i = 0; i < CASE_OBJ; i++) {
    bit = case_obj_getattr(*obj, i);
    newi = indx_wrap(i + inc, CASE_OBJ);
    case_obj_setattr(&obj2, newi, bit);
  }
  *obj = obj2;
}

void case_obj_setfromstr(case_obj_t *obj, char str[CASE_OBJ])
{
  long i;
  case_bit_t val;
  for (i = 0; i < CASE_OBJ; i++) {
    val = ('0' == str[i]) ? 0 : 1;
    case_obj_setattr(obj, i, val);
  }
}

void case_obj_setnum(case_obj_t *obj, long startbit, long bits, long long num)
{
  long place = 2;
  long bit = startbit;
  case_bit_t val;
  long long rem = num;
  do {
    val = rem % place;
    case_obj_setattr(obj, bit, val);
    rem -= val;
    place *= 2;
    bit++;
  } while (bit < bits);
}

long findsmashpoint(case_obj_t obj1, case_obj_t obj2)
{
  long smash = CASE_OBJ - 1;
  case_bit_t bit1;
  case_bit_t bit2;
  bit1 = case_obj_getattr(obj1, smash);
  bit2 = case_obj_getattr(obj2, smash);
  while ((smash > 0) && !bit1 && !bit2) {
    smash--;
    bit1 = case_obj_getattr(obj1, smash);
    bit2 = case_obj_getattr(obj2, smash);
  }
  return smash;
}
