#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "object.h"
#include "tool.h"

void case_object_clear(case_object_t *obj)
{
  *obj = 0;
}

double case_object_comparet(case_object_t obj1, case_object_t obj2)
{
  long bit;
  long correct = 0;
  long total = 1;
  case_bit_t bit1;
  case_bit_t bit2;
  for (bit = 1; bit < 32; bit++) {
    bit1 = case_object_getattr(obj1, bit);
    bit2 = case_object_getattr(obj2, bit);
    if ((1 == bit1) || (1 == bit2)) {
      total++;
      if ((1 == bit1) && (1 == bit2)) {
        correct++;
      }
    }
  }
  return (double) correct / total;
}

double case_object_compareq(case_object_t obj1, case_object_t obj2)
{
  long bit;
  long correct = 0;
  case_bit_t bit1;
  case_bit_t bit2;
  for (bit = 1; bit < 32; bit++) {
    bit1 = case_object_getattr(obj1, bit);
    bit2 = case_object_getattr(obj2, bit);
    if (bit1 == bit2) {
      correct++;
    }
  }
  return (double) correct / 31;
}

double case_object_comparex(case_object_t obj1, case_object_t obj2)
{
  long bit;
  long endbit;
  long correct = 0;
  long total;
  case_bit_t bit1;
  case_bit_t bit2;
  endbit = 31;
  bit1 = case_object_getattr(obj1, endbit);
  bit2 = case_object_getattr(obj2, endbit);
  while ((endbit > 0) && !bit1 && !bit2) {
    endbit--;
    bit1 = case_object_getattr(obj1, endbit);
    bit2 = case_object_getattr(obj2, endbit);
  }
  total = endbit + 1;
  for (bit = 1; bit <= endbit; bit++) {
    bit1 = case_object_getattr(obj1, bit);
    bit2 = case_object_getattr(obj2, bit);
    if (bit1 == bit2) {
      correct++;
    }
  }
  return (double) correct / total;
}

long case_object_getnum(case_object_t obj, long startbit, long bits)
{
  long place = 1;
  long bit;
  long num = 0;
  for (bit = startbit; bit < bits; bit++) {
    num += place * case_object_getattr(obj, bit);
    place *= 2;
  }
  return num;
}

case_bit_t case_object_hastype(case_object_t obj, case_object_t type)
{
  case_bit_t has = 1;
  long idx;
  for (idx = 0; idx < 32; idx++) {
    if (case_object_getattr(type, idx)) {
      if (!case_object_getattr(obj, idx)) {
        has = 0;
        break;
      }
    }
  }
  return has;
}

void case_object_mutate(case_object_t *obj)
{
  long idx = random() % 32;
  case_bit_t val = case_bit_random();
  case_object_setattr(obj, idx, val);
}

void case_object_print(case_object_t obj)
{
  long idx;
  for (idx = 0; idx < 32; idx++) {
    printf("%lu", case_object_getattr(obj, idx));
  }
}

void case_object_randomize(case_object_t *obj)
{
  *obj = random();
}

void case_object_rotate(case_object_t *obj, long inc)
{
  case_object_t obj2;
  long idx;
  long newidx;
  case_bit_t bit;
  for (idx = 0; idx < 32; idx++) {
    bit = case_object_getattr(*obj, idx);
    newidx = tool_wrapidx(idx + inc, 32);
    case_object_setattr(&obj2, newidx, bit);
  }
  *obj = obj2;
}

void case_object_setfromstr(case_object_t *obj, char *str)
{
  long idx;
  case_bit_t val;
  for (idx = 0; idx < 32; idx++) {
    if ('0' == str[idx]) {
      val = 0;
    } else {
      val = 1;
    }
    case_object_setattr(obj, idx, val);
  }
}

void case_object_setnum(case_object_t *obj, long startbit, long bits, long num)
{
  long place = 2;
  long bit = startbit;
  case_bit_t val;
  long rem = num;
  do {
    val = rem % place;
    case_object_setattr(obj, bit, val);
    rem -= val;
    place *= 2;
    bit++;
  } while (bit < bits);
}
