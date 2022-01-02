#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "index.h"
#include "obj.h"

void case_obj_clear(case_obj_t *obj)
{
  *obj = 0;
}

double case_obj_comparet(case_obj_t obj1, case_obj_t obj2)
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

double case_obj_compareq(case_obj_t obj1, case_obj_t obj2)
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

double case_obj_comparex(case_obj_t obj1, case_obj_t obj2)
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

case_bit_t case_obj_hastype(case_obj_t obj, case_obj_t type)
{
  case_bit_t has = 1;
  long idx;
  for (idx = 0; idx < 32; idx++)
    if (case_obj_getattr(type, idx))
      if (!case_obj_getattr(obj, idx)) {
        has = 0;
        break;
      }
  return has;
}

void case_obj_mutate(case_obj_t *obj)
{
  long idx = random() % 32;
  case_bit_t val;
  case_bit_randomize(&val);
  case_obj_setattr(obj, idx, val);
}

void case_obj_print(case_obj_t obj)
{
  long idx;
  for (idx = 0; idx < 32; idx++)
    printf("%lu", case_obj_getattr(obj, idx));
}

void case_obj_randomize(case_obj_t *obj)
{
  *obj = random();
}

void case_obj_rotate(case_obj_t *obj, long inc)
{
  case_obj_t obj2;
  long idx;
  long newidx;
  case_bit_t bit;
  for (idx = 0; idx < 32; idx++) {
    bit = case_obj_getattr(*obj, idx);
    newidx = index_wrap(idx + inc, 32);
    case_obj_setattr(&obj2, newidx, bit);
  }
  *obj = obj2;
}

void case_obj_setfromstr(case_obj_t *obj, char *str)
{
  long idx;
  case_bit_t val;
  for (idx = 0; idx < 32; idx++) {
    val = ('0' == str[idx]) ? 0 : 1;
    case_obj_setattr(obj, idx, val);
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
