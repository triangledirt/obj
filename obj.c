#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "bool.h"
#include "indx.h"
#include "obj.h"

void case_obj_clear(case_obj_t *obj)
{
  *obj = 0;
}

double case_obj_compareequal(case_obj_t obj1, case_obj_t obj2)
{
  long bit;
  long correct = 0;
  case_bit_t bit1;
  case_bit_t bit2;
  for (bit = 1; bit < 32; bit++) {
    bit1 = case_obj_getattr(obj1, bit);
    bit2 = case_obj_getattr(obj2, bit);
    if (bit1 == bit2)
      correct++;
  }
  return (double) correct / 31;
}

double case_obj_comparesmash(case_obj_t obj1, case_obj_t obj2)
{
  long bit;
  long endbit;
  long correct = 0;
  long total;
  case_bit_t bit1;
  case_bit_t bit2;
  endbit = 31;
  bit1 = case_obj_getattr(obj1, endbit);
  bit2 = case_obj_getattr(obj2, endbit);
  while ((endbit > 0) && !bit1 && !bit2) {
    endbit--;
    bit1 = case_obj_getattr(obj1, endbit);
    bit2 = case_obj_getattr(obj2, endbit);
  }
  total = endbit + 1;
  for (bit = 1; bit <= endbit; bit++) {
    bit1 = case_obj_getattr(obj1, bit);
    bit2 = case_obj_getattr(obj2, bit);
    if (bit1 == bit2)
      correct++;
  }
  return (double) correct / total;
}

double case_obj_comparetypes(case_obj_t obj1, case_obj_t obj2)
{
  long bit;
  long correct = 0;
  long total = 1;
  case_bit_t bit1;
  case_bit_t bit2;
  for (bit = 1; bit < 32; bit++) {
    bit1 = case_obj_getattr(obj1, bit);
    bit2 = case_obj_getattr(obj2, bit);
    if ((1 == bit1) || (1 == bit2)) {
      total++;
      if ((1 == bit1) && (1 == bit2))
        correct++;
    }
  }
  return (double) correct / total;
}

long case_obj_getnum(case_obj_t obj, long startbit, long bits)
{
  long place = 1;
  long bit;
  long num = 0;
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
  for (i = 0; i < 32; i++)
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
  i = random() % 32;
  case_bit_randomize(&val);
  case_obj_setattr(obj, i, val);
}

double case_obj_oblivion(case_obj_t obj1, case_obj_t obj2)
{
  double oblivion;
  long match;
  long opposite = 0;
  long i;
  case_bit_t o1bit;
  case_bit_t o2bit;
  for (i = 0; i < 32; i++) {
    o1bit = case_obj_getattr(obj1, i);
    o2bit = case_obj_getattr(obj2, i);
    if (o1bit != o2bit)
      opposite++;
  }
  match = 32 - opposite;
  oblivion = (32 - labs(match - opposite)) / 32;
  return oblivion;
}

void case_obj_obscureclass(case_obj_t *obj)
{
  case_obj_setclass(obj, case_bit_random());
}

void case_obj_print(case_obj_t obj)
{
  long i;
  for (i = 0; i < 32; i++)
    printf("%lu", case_obj_getattr(obj, i));
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
  for (i = 0; i < 32; i++) {
    bit = case_obj_getattr(*obj, i);
    newi = indx_wrap(i + inc, 32);
    case_obj_setattr(&obj2, newi, bit);
  }
  *obj = obj2;
}

void case_obj_setfromstr(case_obj_t *obj, char *str)
{
  long i;
  case_bit_t val;
  for (i = 0; i < 32; i++) {
    val = ('0' == str[i]) ? 0 : 1;
    case_obj_setattr(obj, i, val);
  }
}

void case_obj_setnum(case_obj_t *obj, long startbit, long bits, long num)
{
  long place = 2;
  long bit = startbit;
  case_bit_t val;
  long rem = num;
  do {
    val = rem % place;
    case_obj_setattr(obj, bit, val);
    rem -= val;
    place *= 2;
    bit++;
  } while (bit < bits);
}
